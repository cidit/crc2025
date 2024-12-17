#pragma once
#include <PID_RT.h>
#include "sensors/sensor.hpp"
#include "drives/motor.hpp"
#include "math/angles.hpp"
#include "util/looped.hpp"
#include "util/timer.hpp"
#include "util/misc.hpp"
#include <Encoder.h>


namespace drives
{
    class PrecisionMotor : public Looped
    {

    public:
        //-------------------------- CONST -----------------------------
        /**
         * Depending on which mode the precision motor is on, it will use the encoder differently.
         * When MATCH_RPM, the motor will output as much power as needed to maintain a certain speed.
         * When MATCH_ANGLE, the motor will apply oposite forces to maintain the specific angle of the wheel.
         */
        enum class Mode : int
        {
            MATCH_RPM = 0,
            MATCH_ANGLE = 1,
        };

        static constexpr double TICKS_117 = 1425.1;  //Bras
        static constexpr double TICKS_312 = 537.7; 
        static constexpr double TICKS_1150 = 145.1; //Lanceur, swerve

        //---------------------- CONSTRUCTORS ---------------------------
        PrecisionMotor(Motor m, Encoder &e, double pA, double iA, double dA, double pS, double iS, double dS, int pid_interval, double max_rpm, double ticks_turn)
            : _mode(Mode::MATCH_RPM),
              _motor(m),
              _encoder(e),
              _max_rpm(max_rpm),
              _ticks_turn(ticks_turn),
              _last_enco(e.read()),
              _delai(pid_interval)
        {
            //Config of PID on angle
            _pidA.setOutputRange(-1, 1);
            _pidA.setInterval(pid_interval);
            _pidA.setK(pA, iA, dA);
            _pidA.setPoint(0);

            //Config of PID on speed
            _pidS.setOutputRange(-128, 127);
            _pidS.setInterval(pid_interval);
            _pidS.setK(pS, iS, dS);
            _pidS.setPoint(0);

            _timer = millis();
        }

        /**
         * Must be called in setup
         */
        void begin(){
            set_target_angle(math::Angle::zero());

            _motor.begin();

            _pidS.start();
            _pidA.start();
        }

        //-------------------------- FUNCTIONS -----------------------------
        void loop() override
        {          
            double interval = millis() - _timer;
            if(interval >= _delai){
                _timer = millis();

                auto enco_out = _encoder.read();
                auto distance_travelled = _last_enco - enco_out;

                //Calculate current Angle
                _current_angle = math::Angle::from_ratio(fmod(enco_out, _ticks_turn)/_ticks_turn);

                //Calculate current speed of motor
                auto current_rpm = (distance_travelled /_ticks_turn / (interval/1000.0))*60;

                _last_enco = enco_out;

                //constrain
                if(current_rpm > _max_rpm){
                current_rpm = _max_rpm;
                }else if(current_rpm < -_max_rpm){
                current_rpm = -_max_rpm;
                }
                
                //Compute using current speed
                _inputS = current_rpm;
                
                //Calculate the diff between current and target angle
                _inputA = math::Angle::travel(_current_angle, _target_angle);
            } 

            //Apply right PID
            if(_mode == Mode::MATCH_RPM){
                Serial.println("calcul!!");
                if (_pidS.compute(_inputS)) {
                    _outputS = _pidS.getOutput();

                    _motor.set_power(_outputS);
                }
            }
            else if(_mode == Mode::MATCH_ANGLE){
                if (_pidA.compute(_inputA)) {
                    _outputA = _pidA.getOutput();

                    _motor.set_power_ratio(_outputA);
                }
                //TODO: Dont do 360
                // Command rotation + direction
            }

        }

        /**
         * Set the mode to MATCH_ANGLE
         * @param angle Angle Object
         */
        void set_target_angle(math::Angle angle){
            _mode = Mode::MATCH_ANGLE;
            _target_angle = angle;
        }

        /**
         * Set the mode to MATCH_RPM
         * @param target_rpm Set the target speed in RPM
         */
        void set_target_speed(double target_rpm){
            _mode = Mode::MATCH_RPM;
            _pidS.setPoint(target_rpm);
        }

        /**
         * Configure the PID on error
         * Used when in MATCH_ANGLE mode
         */
        void set_angle_pid(double p, double i, double d){
            _pidA.setK(p, i, d);
        }

        /**
         * Configure the PID on measurement
         * Used when in MATCH_RPM mode
         */
        void set_speed_pid(double p, double i, double d){
            _pidS.setK(p, i, d);
        }

        /**
         * @return Max RPM programmed in the motor
         */
        double get_max_rpm(){
            return _max_rpm;
        }


    private:
        //-------------------------- VARIABLES -----------------------------
        /**
         * These are the parameters for the PID on angle.
         * Used as reference by the PID
         */
        double _setpointA, _inputA, _outputA;
        /**
         * These are the parameters for the PID on speed.
         * Used by the PID, as RPM
         */
        double _setpointS, _inputS, _outputS;

        Mode _mode;
        Motor _motor;
        Encoder &_encoder;

        /** PID for Angle */
        PID_RT _pidA;
        /** PID for Speed */
        PID_RT _pidS;

        double _max_rpm;
        double _ticks_turn;

        double _last_enco;
        int _timer;
        int _delai;

        math::Angle _current_angle, _target_angle;
    };

}