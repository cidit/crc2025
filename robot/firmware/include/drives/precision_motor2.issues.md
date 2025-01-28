#

## TODO: list of potential issues (thank you, gpt-4) (as of Jan 2 2025)

This is a relatively complex piece of code, and there are several potential bugs, logical issues, or areas of improvement in the `PrecisionMotor2` class. Here's a comprehensive analysis of potential problems:

---

### 1. **Initialization of References (`Motor` and `Encoder`)**

- **Potential Bug**: The `_m` and `_e` references are not checked for validity. If null references are passed to the constructor, it will lead to undefined behavior.
- **Fix**: Consider adding assertions or checks in the constructor to ensure valid references are passed.
SUGGESTION: look up online how to do that in cpp. probably involves nullptr

---

### 10. **`get_current_rpm()` Behavior When Disabled**

- **Bug**: As noted in the `FIXME` comment, `get_current_rpm()` does not produce correct results when the system is disabled because `_update_l2ev()` is not called.
- **Fix**: Either return a default or invalid value (e.g., `NaN`) when the system is disabled, or call `_update_l2ev()` conditionally.
SUGGESTION: if and when we start using .readAndReset() on the encoder to fix the overflow issue, this function will need to work differently (if at all in angle mode)
SUGGESTION: make a separate PrecisionMotor::Encoder class that uses the regular encoder under the hood.
DONE: caviat: for the speed to always work, the encoder needs to be polled regularly via it's update method. which kinda makes sense.

---

### 11. **Unhandled Encoder Overflows**

- **Potential Bug**: The encoder readings `_e.read()` are stored in 32-bit integers (`_e_old1`, `_e_old2`). If the encoder overflows, the `_delta_ticks()` calculation may produce incorrect results.
- **Fix**: Use modular arithmetic to handle encoder overflows correctly.
SUGGESTION: use readAndReset() on the encoder. THIS WILL BREAK ANGLE! 0rad must be tracked inteligently.
DONE: ruled out because after some quick maths, the way we use our encoders and stuff, the encoder will overflow in 90 hours if the motor turns at max speed in the same direction.

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
