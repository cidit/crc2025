#

## TODO: gpt-4's opinion on potential issues

Here is an analysis of your `SwerveModule` implementation. While the code looks generally well thought-out, there are a few potential issues and considerations that might require attention:

---

### **2. `pulseIn` Usage**

- **Issue:**
  - `pulseIn` can block for a significant amount of time if no pulse is detected, which could hinder real-time operation.
  - If the pulse length exceeds `MAX_PULSE_LEN * 2`, the code might fail to compute a valid angle.
- **Fix:**
  - Use non-blocking approaches or interrupts for reading the absolute encoder.
  - Handle cases where the pulse exceeds the timeout to avoid unpredictable behavior.
  - Validate the returned pulse value before computing the angle.
SUGGESTION: make encoder external, make it poll on a regular timeframe and get the last value only

---

### **4. `apply_oprev_optimisation`**

- **Issue:**
  - The optimization assumes an instantaneous response to reverse the motor, which might not be feasible due to mechanical inertia or PID delays.
  - No feedback loop ensures the optimization was successfully applied in real-world conditions.
- **Fix:** Include a mechanism to verify if reversing the wheel results in the expected behavior during runtime.
SUGGESTION: ignore this issue

---

### **5. Forward and Reverse Kinematics**

- **Issue:** The reverse kinematics calculation in `_set_speeds` seems fine, but you are using `_pid.getOutput()` for angular velocity without clear units or normalization. This could result in motor commands being incorrectly scaled.
- **Fix:** Ensure `_pid.getOutput()` is properly scaled to match the RPM range of the motors.
DONE (i think): since we're working in angles here, this shouldn't pose any problems. the burden of having a correct output falls in the hands of the pid tunings.
NOT DONE: i just properly understood the question: the output of the PID is a signed percentage [-1, 1] but the `_set_speeds(...)` function takes RPMs, not a percentage of power. the PIDs should accomodate that. (make the pid output infinite? initialize it to some math involving the theoretical max speed of each motor?)

---

### **6. `STEERING_TOLERANCE` Usage**

- **Issue:**
  - Using a fixed tolerance (`STEERING_TOLERANCE = 0.1`) may not account for dynamic conditions, such as higher speeds requiring stricter tolerances.
  - No hysteresis is implemented to prevent oscillations when the angle error hovers around the tolerance boundary.
- **Fix:** Dynamically adjust the tolerance based on speed or implement hysteresis to avoid oscillations.

---

### **8. Dependency on `Vec2D`**

- **Issue:** The use of `Vec2D` assumes it has robust methods like `.angle()` and `.norm()` for polar conversions. If these methods are not implemented or tested thoroughly, they could introduce bugs.
- **Fix:** Ensure `Vec2D` is implemented correctly and can handle edge cases like zero vectors.
SUGGESTION: unit test

---

### **10. Angle Normalization**

- **Issue:** In `apply_oprev_optimisation`, angles are adjusted with `±M_PI` but are not explicitly normalized back into the range `[−π, π]` or `[0, 2π]`.
- **Fix:** Explicitly normalize the angle after applying the optimization to avoid edge cases.
SUGGESTION: not important given the context. either move the function, inline it or just ignore the issue.

---

### **12. Target Angle Update**

- **Issue:** The `set_target` method updates `_target` but doesn't validate or sanitize the input (e.g., ensuring the angle is within valid bounds).
- **Fix:** Add input validation to `set_target`.
SUGGESTION: normalize the vector depending on the theoretical max speed of the motors.

---

### Suggestions for Improvement

- **Unit Tests:** Create test cases for edge cases in angle calculations, motor speed settings, and the optimization logic.
- **Logging:** Add debug logging for critical events like PID activation, angle normalization, and target updates to facilitate troubleshooting.
- **Real-World Validation:** Validate the behavior on hardware to ensure synchronization between the motors, encoder, and PID controllers.

These changes will improve the reliability and maintainability of your `SwerveModule` implementation. Let me know if you'd like any specific fixes demonstrated!
