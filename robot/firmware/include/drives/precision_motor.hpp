#pragma once

#include <PID_v1.h>
#include "sensors/sensor.hpp"
#include "drives/motor.hpp"
#include "math/angles.hpp"
#include "util/looped.hpp"
#include "util/timer.hpp"
#include "util/misc.hpp"
#include <Encoder.h>

// // Define the aggressive and conservative Tuning Parameters
// double aggKp = 4, aggKi = 0.2, aggKd = 1;
// double consKp = 1, consKi = 0.05, consKd = 0.25;

namespace drives
{
    class PrecisionMotor : public Looped
    {

    public:
        //-------------------------- CONST -----------------------------
        /**
         * Depending on which mode the precision motor is on, it will use the encoder differently.
         */
        enum class Mode : int
        {
            /**
             * When matching rpm, the motor will output as much power as needed to maintain a certain speed.
             */
            MATCH_RPM = 0,
            /**
             * When matching angle, the motor will apply oposite forces to maintain the specific angle of the wheel.
             */
            MATCH_ANGLE = 1,
        }; 

        //---------------------- CONSTRUCTORS ---------------------------
        PrecisionMotor(Motor m, Encoder &e, double Ep, double Ei, double Ed, double Mp, double Mi, double Md, uint16_t poll_rate)
            : _mode(Mode::MATCH_ANGLE),
              _motor(m),
              _encoder(e),
              _pidE(&_inputE, &_outputE, &_setpointE, Ep, Ei, Ed, P_ON_E, DIRECT),
              _pidM(&_inputM, &_outputM, &_setpointM, Mp, Mi, Md, P_ON_E, DIRECT),
              _last_polled_position(math::Angle::zero()),
              _polling_timer(ONE_SECOND / poll_rate),
              _target_rps(0)
        {
            //Config of PID on error
            _pidE.SetMode(AUTOMATIC);
            _pidE.SetOutputLimits(-1, 1);
            _pidE.SetSampleTime(_polling_timer._delay);

            //Config of PID on Measurement
            _pidM.SetMode(AUTOMATIC);
            _pidM.SetOutputLimits(-1, 1);
            _pidM.SetSampleTime(_polling_timer._delay);

            set_target_angle(math::Angle::zero()); // initialize for angle control
        }
        // PrecisionMotor(Motor m, sensors::Sensor<math::Angle> &e, double Ep, double Ei, double Ed, double Mp, double Mi, double Md, uint16_t poll_rate)
        //     : _mode(Mode::MATCH_ANGLE),
        //       _motor(m),
        //       _encoder(e),
        //       _pidE(&_inputE, &_outputE, &_setpointE, Ep, Ei, Ed, P_ON_E, DIRECT),
        //       _pidM(&_inputM, &_outputM, &_setpointM, Mp, Mi, Md, P_ON_M, DIRECT),
        //       _last_polled_position(e.getLast()),
        //       _polling_timer(ONE_SECOND / poll_rate)
        // {
        //     //Config of PID on error
        //     _pidE.SetMode(AUTOMATIC);
        //     _pidE.SetOutputLimits(-1, 1);
        //     _pidE.SetSampleTime(_polling_timer._delay);

        //     //Config of PID on Measurement
        //     _pidM.SetMode(AUTOMATIC);
        //     _pidM.SetOutputLimits(-1, 1);
        //     _pidM.SetSampleTime(_polling_timer._delay);

        //     set_target_angle(math::Angle::zero()); // initialize for angle control
        // }

        //-------------------------- FUNCTIONS -----------------------------
        virtual void loop() override
        {
            _motor.loop();

            //Return if not enough time has passed
            auto now = millis();
            if (!_polling_timer.is_time(now)){
                return;
            }

            // _encoder.poll();
            // auto enco_out = _encoder.getLast();
            auto enco_out = math::Angle::from_ratio(fmod(_encoder.read(), 537.7)/537.7);

            if (_mode == Mode::MATCH_ANGLE){
                
                //Calculate the diff between current and target angle
                _inputE = math::Angle::travel(enco_out, _target_angle);

                if (!_pidE.Compute()) {
                    Serial.println("!? - pid didnt compute, but should have.");
                    return;
                }
                // //TODO: Dont do 360
                // // Command rotation + direction
                // Serial.println(enco_out);
                // Serial.println(_target_angle);
                // Serial.println(_inputE);
                // Serial.println(_outputE);
                // Serial.println();
                _motor.set_speed(_outputE);
            }
            else if (_mode == Mode::MATCH_RPM){

                //Calculate current speed of motor
                auto distance_travelled = math::Angle::travel(_last_polled_position, enco_out);
                auto _current_rps = distance_travelled * ONE_SECOND/_polling_timer._delay;
                
                //Compute using currrent speed
                _inputM = _current_rps;
                if (!_pidM.Compute()) {
                    Serial.println("!? - pid didnt compute, but should have.");
                    return;
                }
                _target_rps += _outputM;
                _target_rps = constrain(_target_rps, -1.0, 1.0);
                Serial.println(_current_rps);
                Serial.println(_target_rps);
                Serial.println(_inputM);
                Serial.println(_outputM);
                Serial.println();
                _motor.set_speed(_target_rps);
            }
        }

        /**
         * Set the mode to MATCH_ANGLE
         * @param angle Angle Object
         */
        void set_target_angle(math::Angle angle){
            _mode = Mode::MATCH_ANGLE;
            _target_angle = angle;
            _setpointE = 0;
        }

        /**
         * Set the mode to MATCH_RPM
         * @param rps Set the target speed
         */
        void set_target_speed(double rps){
            _mode = Mode::MATCH_RPM;
            _setpointM = rps;
        }

        /**
         * Configure the PID on error
         * Used when in MATCH_ANGLE mode
         */
        void set_error_pid(double p, double i, double d){
            _pidE.SetTunings(p, i, d);
        }

        /**
         * Configure the PID on measurement
         * Used when in MATCH_RPM mode
         */
        void set_measurement_pid(double p, double i, double d){
            _pidM.SetTunings(p, i, d);
        }


    private:
        //-------------------------- VARIABLES -----------------------------
        /**
         * These are the parameters for the PID on error.
         * Used as reference by the PID
         */
        double _setpointE, _inputE, _outputE;
        /**
         * These are the parameters for the PID on measurement.
         * Used as reference by the PID, as RPS
         */
        double _setpointM, _inputM, _outputM;

        Mode _mode;
        Motor _motor;
        Encoder &_encoder;
        //sensors::Sensor<math::Angle> &_encoder;

        /** PID on error */
        PID _pidE;
        /** PID on measurement */
        PID _pidM;

        math::Angle _last_polled_position;

        /**
         * Handles how often the encoder will be polled.
         * Poll rate must be fast enough so that the wheel doesn't have the time make more than half a turn.
         */
        Timer _polling_timer;

        double _ticks_per_rotation, _current_rps;
        math::Angle _target_angle;
        double _target_rps;
    };

}