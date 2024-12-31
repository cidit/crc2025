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
         * When MATCH_POWER, the motor will output as much power as needed to maintain a certain speed.
         * When MATCH_ANGLE, the motor will apply oposite forces to maintain the specific angle of the wheel.
         */
        enum class Mode : int
        {
            MATCH_POWER = 0,
            MATCH_ANGLE = 1,
        };

        static constexpr double TICKS_117 = 1425.1;  //Bras
        static constexpr double TICKS_312 = 537.7;
        static constexpr double TICKS_1150 = 145.1; //Lanceur, swerve

        const double MAX_MOTEUR_POWER = 127;

        //---------------------- CONSTRUCTORS ---------------------------
        PrecisionMotor(Motor m, Encoder &e, int pid_interval, double max_rpm, double ticks_turn)
            :
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
            _pidA.setK(_KpA, _KiA, _KdA);
            _pidA.setPoint(0);
            _pidA.setPropOnError();

            //Config of PID on speed
            _pidS.setOutputRange(-127, 127);
            _pidS.setInterval(pid_interval);
            _pidS.setK(_KpS, _KiS, _KdS);
            _pidS.setPoint(0);
            _pidS.setPropOnError();

            _timer = millis();
        }

        /**
         * Must be called in setup
         */
        void begin(){
            _motor.begin();
        }

        //-------------------------- FUNCTIONS -----------------------------
        void loop() override
        {
            _pidS.start();
            _pidA.start();
            
            double delta_time = millis() - _timer;
            if(delta_time >= _delai){
                _timer = millis();

                auto enco_out = _encoder.read();
                auto delta_pos = _last_enco - enco_out;

                //Calculate current Angle
                _current_angle = math::Angle::from_ratio(fmod(enco_out, _ticks_turn)/_ticks_turn);

                //Calculate current speed of motor
                double speed = (double)delta_pos / delta_time * 1000.0; //In ticks per sec
                double current_rpm = speed * 60 / _ticks_turn; //In RPM

                _last_enco = enco_out;

                //constrain
                if(current_rpm > _max_rpm){
                    current_rpm = _max_rpm;
                }else if(current_rpm < -_max_rpm){
                    current_rpm = -_max_rpm;
                }
                
                //Compute using current "power"
                _inputS = current_rpm * 127.0 / _max_rpm;
                // Serial.println(" CurrentRPM: "+String(current_rpm) /*+" MaxRPM: "+String(_max_rpm)+" Ticks: "+String(delta_pos)*/+" MaxTicks: "+String(_ticks_turn));
                
                //Calculate the diff between current and target angle
                _inputA = math::Angle::travel(_current_angle, _target_angle);
                //Serial.print(" CurrentAngle: "+String(_current_angle._radians)+" TarAngle: "+String(_target_angle._radians)+" Travel: "+String(_inputA));
            } 

            //Apply right PID
            if(_mode == Mode::MATCH_POWER){
                if (_pidS.compute(_inputS)) {
                    _outputS = _pidS.getOutput();

                    _current_motor_out = constrain(_current_motor_out + _outputS, -127.0, 127.0);

                    //Serial.print(" PIDInS: "+String(_inputS)+" PIDSetS: "+String(_setpointS)+" PIDOutS: "+String(_outputS));
                    _motor.set_power(_current_motor_out);
                }
            }
            else if(_mode == Mode::MATCH_ANGLE){
                if (_pidA.compute(_inputA)) {
                    _outputA = _pidA.getOutput();

                    //Serial.print(" PIDInA: "+String(_inputA)+" PIDSetA: "+String(_pidA.getSetPoint())+" PIDOutA: "+String(_outputA));
                    _motor.set_power(_outputA*MAX_MOTEUR_POWER);
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
         * Set the mode to MATCH_POWER
         * @param target_rpm Set the target power between -128 and 127
         */
        void set_target_power(double target_pow){
            _mode = Mode::MATCH_POWER;
            _setpointS = target_pow;
            _pidS.setPoint(_setpointS);
        }

        /**
         * Configure the PID on error
         * Used when in MATCH_ANGLE mode
         */
        void set_angle_pid(double p, double i, double d){
            _KpA = p;
            _KiA = i;
            _KdA = d;
            _pidA.setK(_KpA, _KiA, _KdA);
        }

        /**
         * Configure the PID on measurement
         * Used when in MATCH_POWER mode
         */
        void set_speed_pid(double p, double i, double d){
            _KpS = p;
            _KiS = i;
            _KdS = d;
            _pidS.setK(_KpS, _KiS, _KdS);
        }

        double get_input(){
            return _inputS;
        }

        void print_params(){
            Serial.print(_pidS.getKp()); Serial.print(", "); Serial.print(_pidS.getKi()); Serial.print(", "); Serial.println(_pidS.getKd());
        }


    // private:
        //-------------------------- VARIABLES -----------------------------
        /**
         * These are the parameters for the PID on angle.
         * Used as reference by the PID
         */
        double _setpointA;
        double _inputA;
        double _outputA;
        double _KpA = 0.8;
        double _KiA = 0.01;
        double _KdA = 0.001;

        /**
         * These are the parameters for the PID on speed.
         * Used by the PID, as RPM
         */
        double _setpointS;
        double _inputS;
        double _outputS;
        double _KpS = 0.021;
        double _KiS = 0.001;
        double _KdS = 0.0001;
        double _current_motor_out = 0;

        Mode _mode = Mode::MATCH_POWER;
        Motor _motor;
        Encoder &_encoder;

        /** PID for Angle */
        PID_RT _pidA;
        /** PID for Speed */
        PID_RT _pidS;

        double _max_rpm;
        double _ticks_turn;

        double _last_enco;
        uint32_t _timer;
        int _delai = 10;

        math::Angle _current_angle;
        math::Angle _target_angle = math::Angle::zero();
    };

}