#pragma once

#include <Arduino.h>
#include <PID_RT.h>
#include "sensors/sensor.hpp"
#include "drives/motor.hpp"
#include "math/angles.hpp"
#include "util/lifecycle.hpp"
#include "util/timer.hpp"
#include "util/misc.hpp"
#include <Encoder.h>

class PrecisionMotor : public Lifecycle
{
public:
    static const uint32_t DEFAULT_POLL_FREQ = 50; // hz
    enum class Mode
    {
        MATCH_ANGLE,
        MATCH_SPEED
    };

    Motor &_m;
    Encoder &_e;

    /**
     * INPUT: current rpm
     * SETPOIT: target rpm
     * OUTPUT: power variation. expects between -1000 and 1000 to accomodate KP,KI,KD
     *
     * check constructor for additionnal base expectations
     */
    PID_RT _pid_speed;
    int32_t _e_old1, _e_old2;

    /**
     * INPUT: difference between current and target
     * SETPOINT: always 0 because we want to minimise the angle difference
     * OUTPUT: direct power of the motor to reach the desired angle between 0 and 1
     * (can also be interpreted as the power needed to resist change, I.E. this is
     * not a variation of power like it is for the speed)
     *
     * check constructor for additionnal base expectations
     */
    PID_RT _pid_angle;
    double _target_angle;

    double _tpt;
    Mode _mode;
    boolean _enabled;

    PrecisionMotor(Motor &m, Encoder &e, double ticks_per_turn)
        : _m(m),
          _e(e),
          _pid_speed(),
          _e_old1(0),
          _e_old2(0),
          _pid_angle(),
          _target_angle(0),
          _tpt(ticks_per_turn),
          _mode(Mode::MATCH_ANGLE), // doesnt matter, pids are not started anyways
          _enabled(false)
    {
        // setting sane defaults for our pids

        auto poll_interval = ONE_SECOND / DEFAULT_POLL_FREQ;
        {
            /* setting up speed pid */
            _pid_speed.setK(0, 0, 0);
            _pid_speed.setInterval(poll_interval);
            _pid_speed.setPoint(0);
            _pid_speed.setPropOnError();
            _pid_speed.setReverse(true);
            // we set the PID output to a big range to make KP,KI,KD bigger
            // numbers. makes the tuning easier for Guillaume.
            _pid_speed.setOutputRange(-1000, 1000);
        }
        {
            /* setting up angle pid */
            _pid_angle.setK(0, 0, 0);
            _pid_angle.setInterval(poll_interval);
            _pid_angle.setPoint(0);
            _pid_angle.setPropOnError();
            _pid_angle.setReverse(false);
            _pid_angle.setOutputRange(-1, 1);
        }
    }

    void begin() override
    {
        _m.begin();
    }

    void update() override
    {
        auto speed_compute = _pid_speed.compute(get_current_rpm());
        auto angle_compute = _pid_angle.compute(
            math::Angle::travel(get_current_angle(), _target_angle));

        if (speed_compute && angle_compute)
        {
            Serial.print("!!! invalid state error: both PIDs were computed");
            return;
        }

        if (speed_compute || angle_compute)
        {
            _update_l2ev();
        }
        if (speed_compute)
        {
            auto new_power = _m._last_power + _pid_output_to_percentage(_pid_speed);
            _m.set_power_ratio(new_power);
        }
        else if (angle_compute)
        {
            _m.set_power_ratio(_pid_angle.getOutput());
        }
    }

    double get_current_rpm()
    {
        // FIXME: will not output correct values if PM is not enabled
        // because _update_l2ev() never gets called.
        auto interval = _mode == Mode::MATCH_SPEED
                            ? _pid_speed.getInterval()
                            : _pid_angle.getInterval();
        auto hz = ONE_SECOND / interval;
        auto delay_mins = hz * 60;
        return delay_mins * (_delta_ticks() / _tpt);
    }

    double get_current_angle()
    {
        auto e_curr = _e.read();
        return math::Angle::from_ratio(e_curr / _tpt)._radians;
    }

    void set_target_rpm(float rpm)
    {
        _mode = Mode::MATCH_SPEED;
        _pid_speed.setPoint(rpm);
        _set_active_pid();
    }

    void set_target_angle(float angle)
    {
        _mode = Mode::MATCH_ANGLE;
        // validate the angle before saving it
        _target_angle = math::Angle::from_rad(angle)._radians;
        _set_active_pid();
    }

    void enable(bool enable)
    {
        _enabled = enable;
        if (_enabled)
        {
            _set_active_pid();
        }
        else
        {
            pid_soft_reset(_pid_angle);
            pid_soft_reset(_pid_speed);
            _m.set_power_ratio(0);
        }
    }

    /**
     * update last 2 encoder vals
     */
    void _update_l2ev()
    {
        _e_old2 = _e_old1;
        _e_old1 = _e.read();
    }

    int32_t _delta_ticks()
    {
        return _e_old1 - _e_old2;
    }

    double _pid_output_to_percentage(PID_RT &pid)
    {
        // FIXME: this is only useful for the speed pid since its the only
        // one that benefits from the extended output range.
        return pid.getOutput() / 1000;
    }

    void _set_active_pid()
    {
        auto &to_start = _mode == Mode::MATCH_ANGLE
                             ? _pid_angle
                             : _pid_speed;
        auto &to_stop = _mode == Mode::MATCH_ANGLE
                            ? _pid_speed
                            : _pid_angle;

        pid_soft_reset(to_stop);
        if (!to_start.isRunning())
        {
            to_start.start();
        }
    }
};

// TODO: list of potential issues (thank you, gpt-4) (as of Jan 2 2025)
/*

This is a relatively complex piece of code, and there are several potential bugs, logical issues, or areas of improvement in the `PrecisionMotor2` class. Here's a comprehensive analysis of potential problems:

---

### 1. **Initialization of References (`Motor` and `Encoder`)**
   - **Potential Bug**: The `_m` and `_e` references are not checked for validity. If null references are passed to the constructor, it will lead to undefined behavior.
   - **Fix**: Consider adding assertions or checks in the constructor to ensure valid references are passed.

---

### 2. **Use of `boolean` Instead of `bool`**
   - **Issue**: `boolean` is a legacy Arduino type and not standard C++. Use `bool` instead for better portability and compliance with modern C++ standards.
   - **Fix**: Replace `boolean _enabled;` with `bool _enabled;`.

---

### 3. **Encapsulation of Private Variables**
   - **Potential Bug**: Direct manipulation of `_m._last_power` in `loop()` breaks encapsulation. If the internal structure of `Motor` changes, this could lead to errors.
   - **Fix**: Use a getter or setter method from the `Motor` class to access or modify its state.

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

---

### 11. **Unhandled Encoder Overflows**
   - **Potential Bug**: The encoder readings `_e.read()` are stored in 32-bit integers (`_e_old1`, `_e_old2`). If the encoder overflows, the `_delta_ticks()` calculation may produce incorrect results.
   - **Fix**: Use modular arithmetic to handle encoder overflows correctly.

---

### 12. **Non-Synchronous `begin()` and `loop()`**
   - **Issue**: The `begin()` function initializes the motor, but the encoder (`_e`) is not initialized. If the encoder requires initialization, it must be done explicitly.
   - **Fix**: Add a call to an encoder initialization function (if required).
DONE: no action needed, the encoder doesnt need to be begun.
---

### 13. **Potential Memory Usage Problems**
   - **Issue**: The code uses `Serial.print()` for debugging, which can increase memory usage on constrained Arduino devices.
   - **Fix**: Use lightweight debugging mechanisms or conditionally compile debug logs.

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

### Summary of Key Fixes:
- Add validity checks for `Motor` and `Encoder` references.
- Replace `boolean` with `bool`.
- Protect `_m._last_power` with a proper getter/setter.
- Handle invalid states in `loop()` more robustly.
- Avoid division by zero in `get_current_rpm()`.
- Handle encoder overflows and ensure proper scaling for `_pid_output_to_percentage()`.
- Refactor verbose and repetitive code in `_reset_PIDs()`.

By addressing these issues, the class would become more robust, maintainable, and less prone to bugs.

*/