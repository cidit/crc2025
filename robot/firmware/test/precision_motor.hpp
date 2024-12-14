#pragma once

#include "math.h"
#include "math/angles.hpp"
#include "util/looped.hpp"
#include "util/misc.hpp"
#include <PID_RT.h>
#include <Encoder.h>

/**
 * revised version
 */
class PrecisionMotor: public Looped {
    public:
        enum class Mode {
            ANGLE,
            SPEED,
        };

        void loop();

        void init(PID_RT pid, Mode mode, float tick_per_rotation);
        void setValue(double val);
        void setMode(Mode mode);
        void setTunings(PIDTunings tunings);

    private:
        Mode _mode;
        PID_RT _pid;
        Encoder _enco;
        float _tpr;
        double _targetAngle = 0;
        double _targetRPM = 0;

};