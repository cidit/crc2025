#

## TODO: list of potential issues (thank you, gpt-4) (as of Jan 2 2025)

This is a relatively complex piece of code, and there are several potential bugs, logical issues, or areas of improvement in the `PrecisionMotor2` class. Here's a comprehensive analysis of potential problems:

---

### 1. **Initialization of References (`Motor` and `Encoder`)**

- **Potential Bug**: The `_m` and `_e` references are not checked for validity. If null references are passed to the constructor, it will lead to undefined behavior.
- **Fix**: Consider adding assertions or checks in the constructor to ensure valid references are passed.
SUGGESTION: look up online how to do that in cpp. probably involves nullptr

---

### 3. **Encapsulation of Private Variables**

- **Potential Bug**: Direct manipulation of `_m._last_power` in `loop()` breaks encapsulation. If the internal structure of `Motor` changes, this could lead to errors.
- **Fix**: Use a getter or setter method from the `Motor` class to access or modify its state.
SUGGESTION: implement that directly on the Motor class.
DONE: fixed it

---

### 4. **Invalid State Error Handling**

- **Potential Bug**: In `loop()`, if both PIDs compute at the same time (`speed_compute && angle_compute`), the code only logs an error but does not handle the invalid state properly.
- **Fix**: Consider taking corrective action, such as disabling the motor or resetting the system, instead of just logging an error.

---

### 5. **Possible Division by Zero in `get_current_rpm()`**

- **Bug**: If `interval` is `0` (e.g., due to a misconfigured PID interval), the division `ONE_SECOND / interval` will cause undefined behavior.
- **Fix**: Add a check to ensure `interval` is non-zero before performing the division.
DONE: added apropriate guard clause

---

### 6. **Misaligned Units in `_pid_output_to_percentage()`**

- **Issue**: `_pid_output_to_percentage()` assumes that the output range of `_pid_speed` is always `[-1000, 1000]`. If the range changes, this logic will break.
- **Fix**: Use `getOutputRange()` from the PID class (if available) to dynamically calculate the scaling factor.
DONE: fixed it. still assumes that the output range is centered on zero tho

---

### 10. **`get_current_rpm()` Behavior When Disabled**

- **Bug**: As noted in the `FIXME` comment, `get_current_rpm()` does not produce correct results when the system is disabled because `_update_l2ev()` is not called.
- **Fix**: Either return a default or invalid value (e.g., `NaN`) when the system is disabled, or call `_update_l2ev()` conditionally.
SUGGESTION: if and when we start using .readAndReset() on the encoder to fix the overflow issue, this function will need to work differently (if at all in angle mode)
SUGGESTION: make a separate PrecisionMotor::Encoder class that uses the regular encoder under the hood.

---

### 11. **Unhandled Encoder Overflows**

- **Potential Bug**: The encoder readings `_e.read()` are stored in 32-bit integers (`_e_old1`, `_e_old2`). If the encoder overflows, the `_delta_ticks()` calculation may produce incorrect results.
- **Fix**: Use modular arithmetic to handle encoder overflows correctly.
SUGGESTION: use readAndReset() on the encoder. THIS WILL BREAK ANGLE! 0rad must be tracked inteligently.

---

### 13. **Potential Memory Usage Problems**

- **Issue**: The code uses `Serial.print()` for debugging, which can increase memory usage on constrained Arduino devices.
- **Fix**: Use lightweight debugging mechanisms or conditionally compile debug logs.
SUGGESTION: introduce a logger?
DONE: temporary fix behind a build flag

---

### 14. **Lack of Error Recovery Mechanisms**

- **Issue**: Errors like invalid states (e.g., both PIDs computed simultaneously) or motor/encoder malfunctions are not recoverable.
- **Fix**: Add error recovery mechanisms (e.g., resetting PIDs, reinitializing the motor/encoder).
DONE: disabled the pmotor when encountering an invalid state.

---

### Summary of Key Fixes

- Add validity checks for `Motor` and `Encoder` references.
- Replace `boolean` with `bool`.
- Protect `_m._last_power` with a proper getter/setter.
- Handle invalid states in `loop()` more robustly.
- Avoid division by zero in `get_current_rpm()`.
- Handle encoder overflows and ensure proper scaling for `_pid_output_to_percentage()`.
- Refactor verbose and repetitive code in `_reset_PIDs()`.

By addressing these issues, the class would become more robust, maintainable, and less prone to bugs.
