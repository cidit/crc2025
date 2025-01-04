#pragma once
#include "drives/precision_motor2.hpp"
#include "math/vectors.hpp"

using math::cartesian::Vec2D;

class SwerveModule : public Lifecycle
{
public:
    /**
     * the the angle error (in radians, plus or minus) at which the translation speed
     * will be taken into account
     */
    static const auto STEERING_TOLERANCE = .1;

    PrecisionMotor &_pma, &_pmb;
    pin_t _e_p;
    PID_RT _pid; // for angle of swerve
    Vec2D _target;
    double _mtwr;

    SwerveModule(
        PrecisionMotor &pma,
        PrecisionMotor &pmb,
        pin_t abs_enco_pin,
        /**
         * the motor to wheel ratio is how many turns the wheel does
         * for one turn of the ring gear. our ratio is 2/3 because the
         * inner gear has a ratio of 1:2 with the ring gear and the
         * bevel gear on the wheel has a ratio of 3:1 with the inner gear.
         */
        double motor_to_wheel_ratio = 2. / 3.)
        : _pma(pma),
          _pmb(pmb),
          _e_p(abs_enco_pin),
          _pid(),
          _target(0, 0),
          _mtwr(motor_to_wheel_ratio)
    {
        // TODO: maybe make a function that resets the pid to a sane state like for the
        // precision motor
        _pid.setInterval(20);
        _pid.setPoint(0);
        _pid.setPropOnError();
        _pid.setReverse(false);
        _pid.setOutputRange(-1, 1);
    }

    void begin() override
    {
        _pma.begin();
        _pmb.begin();
    }

    void update() override
    {
        // TODO: unimplemented
        const auto t_angle = _target.angle();
        const auto c_angle = get_current_angle();
        const auto travel = math::Angle::travel(c_angle, t_angle);
        const auto oprev = apply_oprev_optimisation(travel);

        if (!_pid.compute(oprev.travel))
        {
            return;
        }

        const auto t_lin_v = oprev.travel > STEERING_TOLERANCE
                           ? 0
                           : _target.norm() * _mtwr;

        /*
        FORWARD KINEMATICS
        linear velocity: v=(a+b)/2
        angular velocity: Δω=a-b
        REVERSE KINEMATICS
        a=v+(Δω​/2)
        b=v−(Δω/2)
        */
        const auto angular_v = _pid.getOutput(); // rpms
        _set_speeds(
            t_lin_v + _pid.getOutput() / 2,
            t_lin_v - _pid.getOutput() / 2);
    }

    /**
     * sets the target angle and speed we want to reach
     * @param target a vec that encodes a direction as an angle and a speed in RPMs
     */
    void set_target(const Vec2D target)
    {
        _target = target;
    }

    double get_current_angle()
    {
        // TODO: handle the encoder externally so that the pulse length can be ajusted if need be.
        const const auto MAX_PULSE_LEN = 4160.0;
        const double pulse = pulseIn(_e_p, HIGH, MAX_PULSE_LEN * 2); // TODO: if there are issues later, check the pulseIn
        return (pulse / MAX_PULSE_LEN) * (2 * M_PI);
    }

    void enable(const bool enable)
    {
        _pma.enable(enable);
        _pmb.enable(enable);
        if (enable)
        {
            _pid.start();
        }
        else
        {
            _pid.stop();
            pid_soft_reset(_pid);
        }
    }

    /**
     * immediately sets the speed of the motors.
     */
    void _set_speeds(const double rpma, const double rpmb)
    {
        _pma.set_target_rpm(rpma);
        _pmb.set_target_rpm(rpmb);
    }
};

/**
 * results of the oposite & reverse optimisation
 */
struct oprev_result
{
    // reversed tells you if the wheel should be reversed or not. (if true, the optimisation was done)
    bool reverse;
    // travel is in angles, gets optimised if reverse is true
    double travel;
};

/**
 * check if can do the oposite & reverse optimisation
 * @param angle zeroed travel angle
 */
oprev_result apply_oprev_optimisation(double angle)
{
    const auto reversed = abs(angle) > M_PI_2;
    if (reversed)
    {
        angle += angle > 0
                     ? -M_PI
                     : +M_PI;
    }
    return (oprev_result){
        .reverse = reversed,
        .travel = angle,
    };
}

// TODO: gpt-4's opinion on potential issues
/*

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

---

### **3. Encoder Direction Handling**
- **Issue:** If the encoder is not handling direction changes, the reported angle could be invalid or incomplete.
- **Fix:** Add logic to track the encoder's direction or rely on differential readings from the motors if supported.

---

### **4. `apply_oprev_optimisation`**
- **Issue:**
  - The optimization assumes an instantaneous response to reverse the motor, which might not be feasible due to mechanical inertia or PID delays.
  - No feedback loop ensures the optimization was successfully applied in real-world conditions.
- **Fix:** Include a mechanism to verify if reversing the wheel results in the expected behavior during runtime.

---

### **5. Forward and Reverse Kinematics**
- **Issue:** The reverse kinematics calculation in `_set_speeds` seems fine, but you are using `_pid.getOutput()` for angular velocity without clear units or normalization. This could result in motor commands being incorrectly scaled.
- **Fix:** Ensure `_pid.getOutput()` is properly scaled to match the RPM range of the motors.

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

---

### **8. Dependency on `Vec2D`**
- **Issue:** The use of `Vec2D` assumes it has robust methods like `.angle()` and `.norm()` for polar conversions. If these methods are not implemented or tested thoroughly, they could introduce bugs.
- **Fix:** Ensure `Vec2D` is implemented correctly and can handle edge cases like zero vectors.

---

### **9. Missing Implementation for `update`**
- **Issue:** The core `update` function lacks a concrete implementation for controlling the swerve module. This is marked as `TODO`.
- **Fix:** Implement the logic to control the swerve module based on `_target`, current angle, and PID outputs.

---

### **10. Angle Normalization**
- **Issue:** In `apply_oprev_optimisation`, angles are adjusted with `±M_PI` but are not explicitly normalized back into the range `[−π, π]` or `[0, 2π]`.
- **Fix:** Explicitly normalize the angle after applying the optimization to avoid edge cases.

---

### **11. Initialization Order**
- **Issue:** `_mtwr` is initialized with `2 / 3` in the constructor. In integer division, this evaluates to `0`.
- **Fix:** Use `2.0 / 3.0` for proper floating-point division.

---

### **12. Target Angle Update**
- **Issue:** The `set_target` method updates `_target` but doesn't validate or sanitize the input (e.g., ensuring the angle is within valid bounds).
- **Fix:** Add input validation to `set_target`.

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

*/