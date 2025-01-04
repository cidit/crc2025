#

## TODO: list of potential issues (thank you, gpt-4) (as of Jan 2 2025)

This is a relatively complex piece of code, and there are several potential bugs, logical issues, or areas of improvement in the `PrecisionMotor2` class. Here's a comprehensive analysis of potential problems:

---

### 1. **Initialization of References (`Motor` and `Encoder`)**

- **Potential Bug**: The `_m` and `_e` references are not checked for validity. If null references are passed to the constructor, it will lead to undefined behavior.
- **Fix**: Consider adding assertions or checks in the constructor to ensure valid references are passed.
SUGGESTION: look up online how to do that in cpp. probably involves nullptr

---

### 2. **Use of `boolean` Instead of `bool`**

- **Issue**: `boolean` is a legacy Arduino type and not standard C++. Use `bool` instead for better portability and compliance with modern C++ standards.
- **Fix**: Replace `boolean _enabled;` with `bool _enabled;`.
DONE

---

### 3. **Encapsulation of Private Variables**

- **Potential Bug**: Direct manipulation of `_m._last_power` in `loop()` breaks encapsulation. If the internal structure of `Motor` changes, this could lead to errors.
- **Fix**: Use a getter or setter method from the `Motor` class to access or modify its state.
SUGGESTION: implement that directly on the Motor class.

---

### 4. **Invalid State Error Handling**

- **Potential Bug**: In `loop()`, if both PIDs compute at the same time (`speed_compute && angle_compute`), the code only logs an error but does not handle the invalid state properly.
- **Fix**: Consider taking corrective action, such as disabling the motor or resetting the system, instead of just logging an error.

---

### 5. **Possible Division by Zero in `get_current_rpm()`**

- **Bug**: If `interval` is `0` (e.g., due to a misconfigured PID interval), the division `ONE_SECOND / interval` will cause undefined behavior.
- **Fix**: Add a check to ensure `interval` is non-zero before performing the division.

---

### 6. **Misaligned Units in `_pid_output_to_percentage()`**

- **Issue**: `_pid_output_to_percentage()` assumes that the output range of `_pid_speed` is always `[-1000, 1000]`. If the range changes, this logic will break.
- **Fix**: Use `getOutputRange()` from the PID class (if available) to dynamically calculate the scaling factor.

---

### 7. **Incorrect Behavior When `MATCH_ANGLE` or `MATCH_SPEED` Mode is Switched**

- **Issue**: `_set_active_pid()` starts the new PID but does not reset or reinitialize relevant state variables. This could cause erratic behavior when switching modes.
- **Fix**: Add logic to reset or reinitialize state variables (e.g., `_e_old1`, `_e_old2`) when switching between modes.
DONE: replaced `to_stop.stop()` with a call to `pid_soft_reset`

---

### 8. **Magic Numbers for PID Intervals**

- **Issue**: The interval `20` is hardcoded in `_reset_PIDs()`. This could make the code less maintainable.
- **Fix**: Use a named constant or configurable parameter for the PID interval.
DONE: created a few constants and shit

---

### 9. **Incorrect Use of `get_current_angle()`**

- **Bug**: In `get_current_angle()`, the division `e_curr / _tpt` performs integer division if `_tpt` is an integer, which may lead to incorrect results.
- **Fix**: Ensure `_tpt` is a floating-point value, or cast it to `double` before performing the division.
DONE: no action needed because _tpt is already a double. gpt4 halucination.

---

### 10. **`get_current_rpm()` Behavior When Disabled**

- **Bug**: As noted in the `FIXME` comment, `get_current_rpm()` does not produce correct results when the system is disabled because `_update_l2ev()` is not called.
- **Fix**: Either return a default or invalid value (e.g., `NaN`) when the system is disabled, or call `_update_l2ev()` conditionally.
SUGGESTION: if and when we start using .readAndReset() on the encoder to fix the overflow issue, this function will need to work differently (if at all in angle mode)

---

### 11. **Unhandled Encoder Overflows**

- **Potential Bug**: The encoder readings `_e.read()` are stored in 32-bit integers (`_e_old1`, `_e_old2`). If the encoder overflows, the `_delta_ticks()` calculation may produce incorrect results.
- **Fix**: Use modular arithmetic to handle encoder overflows correctly.
SUGGESTION: use readAndReset() on the encoder. THIS WILL BREAK ANGLE! 0 must be tracked inteligently.

---

### 12. **Non-Synchronous `begin()` and `loop()`**

- **Issue**: The `begin()` function initializes the motor, but the encoder (`_e`) is not initialized. If the encoder requires initialization, it must be done explicitly.
- **Fix**: Add a call to an encoder initialization function (if required).
DONE: no action needed, the encoder doesnt need to be begun.

---

### 13. **Potential Memory Usage Problems**

- **Issue**: The code uses `Serial.print()` for debugging, which can increase memory usage on constrained Arduino devices.
- **Fix**: Use lightweight debugging mechanisms or conditionally compile debug logs.
SUGGESTION: introduce a logger?

---

### 14. **Lack of Error Recovery Mechanisms**

- **Issue**: Errors like invalid states (e.g., both PIDs computed simultaneously) or motor/encoder malfunctions are not recoverable.
- **Fix**: Add error recovery mechanisms (e.g., resetting PIDs, reinitializing the motor/encoder).

---

### 15. **General Code Readability and Maintainability**

- **Observation**: The `_reset_PIDs()` function is verbose and difficult to maintain due to the repeated logic for resetting PIDs.
- **Fix**: Refactor the logic to avoid duplication, possibly by creating a helper function to reset a single PID.
DONE: created helper function pid_soft_reset

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
