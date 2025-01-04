#

## TODO: gpt-4's opinion on potential issues

Here is an analysis of your `SwerveModule` implementation. While the code looks generally well thought-out, there are a few potential issues and considerations that might require attention:

---

### **1. PID Initialization**

- **Issue:** The `_pid` is initialized with default parameters that may not be suitable for your specific application. Additionally, no reset mechanism exists like in the `PrecisionMotor` implementation.
- **Fix:** Implement a `_reset_PID()` method similar to the one in `PrecisionMotor2` to ensure sane defaults for `_pid`.
- DONE: implemented a pid_soft_reset function in utils.

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

### **3. Encoder Direction Handling**

- **Issue:** If the encoder is not handling direction changes, the reported angle could be invalid or incomplete.
- **Fix:** Add logic to track the encoder's direction or rely on differential readings from the motors if supported.
DONE (i think): the encoder does'nt need to handle direction changes as it is absolute.

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

---

### **6. `STEERING_TOLERANCE` Usage**

- **Issue:**
  - Using a fixed tolerance (`STEERING_TOLERANCE = 0.1`) may not account for dynamic conditions, such as higher speeds requiring stricter tolerances.
  - No hysteresis is implemented to prevent oscillations when the angle error hovers around the tolerance boundary.
- **Fix:** Dynamically adjust the tolerance based on speed or implement hysteresis to avoid oscillations.

---

### **7. Lack of Safety Checks**

- **Issue:**
  - `_set_speeds` directly sets RPM values without bounds checking.
  - Overly high or low RPMs could cause damage to the motors or result in unintended behavior.
- **Fix:** Add range validation for `rpma` and `rpmb` in `_set_speeds`.
SUGGESTION: apply that fix in the PrecisionMotor class instead
DONE: added a `_max_rmp` property in the `PrecisionMotor` class that limits the target speed in `set_target_rpm`

---

### **8. Dependency on `Vec2D`**

- **Issue:** The use of `Vec2D` assumes it has robust methods like `.angle()` and `.norm()` for polar conversions. If these methods are not implemented or tested thoroughly, they could introduce bugs.
- **Fix:** Ensure `Vec2D` is implemented correctly and can handle edge cases like zero vectors.
SUGGESTION: unit test

---

### **9. Missing Implementation for `update`**

- **Issue:** The core `update` function lacks a concrete implementation for controlling the swerve module. This is marked as `TODO`.
- **Fix:** Implement the logic to control the swerve module based on `_target`, current angle, and PID outputs.
DONE: chatgpt hallucination. the implementation is there, but a todo was left hanging

---

### **10. Angle Normalization**

- **Issue:** In `apply_oprev_optimisation`, angles are adjusted with `±M_PI` but are not explicitly normalized back into the range `[−π, π]` or `[0, 2π]`.
- **Fix:** Explicitly normalize the angle after applying the optimization to avoid edge cases.
SUGGESTION: not important given the context. either move the function, inline it or just ignore the issue.

---

### **11. Initialization Order**

- **Issue:** `_mtwr` is initialized with `2 / 3` in the constructor. In integer division, this evaluates to `0`.
- **Fix:** Use `2.0 / 3.0` for proper floating-point division.
DONE: fixed that in an earlier commit

---

### **12. Target Angle Update**

- **Issue:** The `set_target` method updates `_target` but doesn't validate or sanitize the input (e.g., ensuring the angle is within valid bounds).
- **Fix:** Add input validation to `set_target`.
SUGGESTION: normalize the vector depending on the theoretical max speed of the motors.

---

### **13. Handling of `_pid.start()` and `_pid.stop()`**

- **Issue:** If `_pid` is started or stopped frequently, it could lead to abrupt behavior in the system.
- **Fix:** Smooth transitions when enabling or disabling the PID controller.

---

### Suggestions for Improvement

- **Unit Tests:** Create test cases for edge cases in angle calculations, motor speed settings, and the optimization logic.
- **Logging:** Add debug logging for critical events like PID activation, angle normalization, and target updates to facilitate troubleshooting.
- **Real-World Validation:** Validate the behavior on hardware to ensure synchronization between the motors, encoder, and PID controllers.

These changes will improve the reliability and maintainability of your `SwerveModule` implementation. Let me know if you'd like any specific fixes demonstrated!
