#pragma once

#include "math.h"
#include "math/vectors.hpp"
#include <PID_RT.h>
using math::cartesian::Vec2D;
#include "drives/precision_motor.hpp"
#include "util/looped.hpp"
#include <CrcLib.h>

class SwerveModule : public Looped
{
    public:
        //---------------------- CONSTANTS ---------------------------
        enum class Direction
        {
            CLOCKWISE,
            COUNTERCLOCKWISE,
        };

        struct TravelParam
        {
            double diff;
            Direction wheel; // if the WHEEL of the swerve reversed FWD/BWD
            Direction dir; // the direction of the rotation
        };

        const double MAX_MOTEUR_POWER = 127;
        
        //---------------------- CONSTRUCTORS ---------------------------
        SwerveModule(drives::PrecisionMotor motorH, drives::PrecisionMotor motorB, double abs_enco_pin)
            : _motorH(motorH),
            _motorB(motorB),
            _abs_enco_pin(abs_enco_pin)
        {
            _pid.setPoint(0);
            _pid.setOutputRange(-1.0, 1.0);
            _pid.setK(0.5, 0.01, 0.001);
            _pid.setInterval(10);
        }

        /**
         * Initialise the default values for the calculations
         * Must be call in Setup()
         */
        void begin(){
            Serial.println(" > Init Swerve");
            
            _motorB.begin();
            _motorH.begin();

            _motorB.set_target_power(0);
            _motorH.set_target_power(0);

            _pid.start();

            //CrcLib::InitializePwmOutput(_abs_enco_pin);
        }

        //-------------------------- PUBLICS -----------------------------
        /**
         * Calculate the power vector (translation, angular) of a Swerve module
         * and set the power of the motors accorddingly
         * @param _target_angle Desired angle of wheel in rads (0 to 2pi)
         * @param _trans_power Multiplication factor for translation power between -1.0 and 1.0
         * @return done - True: Operation completed succesfully, False: Problem
         */
        void loop() override
        {
            Serial.print(" AbsPin: "+String(_abs_enco_pin));
            Vec2D vector = calculate();
            set_motor_powers(vector);
        }

        /**
         * Calculate the power vector (translation, angular) of a Swerve module
         * @return vecPower - {x=translationComponent, y=angularComponent}
         */
        Vec2D calculate(){
            //Set diff and dir in _moveParam
            double currentAngle = get_current_angle();
            get_diff_angle(currentAngle);

            //Find the absolute components of the power vector of the entire module
            if(_pid.compute(get_PID_angle(_moveParam.diff)) && _trans_power != 0){
                _vecPower.set_y(fabs(_pid.getOutput()));
                _vecPower.set_x(get_translation_component(fabs(_moveParam.diff), fabs(_trans_power)));
            }
            else if(_trans_power == 0){ //If the joy is not in use
                _vecPower.set_x(0);
                _vecPower.set_y(0);
            }

            //Apply the good sign
            switch(_moveParam.dir){
                case SwerveModule::Direction::CLOCKWISE:
                    _vecPower.set_y(fabs(_vecPower.y()));
                    break;
                case SwerveModule::Direction::COUNTERCLOCKWISE:
                    _vecPower.set_y(-fabs(_vecPower.y()));
                    break;
            }

            //Determine the sign of the translation component depending on current and target angle
            if(_target_angle <= M_PI){
                //Forward
                if(currentAngle <= M_PI){
                _vecPower.set_x(fabs(_vecPower.x()));
                }else{
                _vecPower.set_x(-fabs(_vecPower.x()));
                }
                
            }else{
                //Backward
                if(currentAngle <= M_PI){
                    _vecPower.set_x(-fabs(_vecPower.x()));
                }else{
                    _vecPower.set_x(fabs(_vecPower.x()));
                }
            }


            return _vecPower;
        }

        /**
         * Set the power of the motors
         * - Si les valeurs de moteurs sont + et + -> tourne horaire
         * - Si les valeurs de moteurs sont - et - -> tourne anti-horaire
         * - Si les valeurs de moteurs sont + et - ->
         * - Si les valeurs de moteurs sont - et + ->
         * @param powerVector Vector2D with components between -1.0 and 1.0
         */
        void set_motor_powers(Vec2D powerVector){

            double powerB = powerVector.y() + powerVector.x();
            double powerA = powerVector.y() - powerVector.x();

            // double max_rpm_h = _motorH.get_max_rpm();
            // double max_rpm_b = _motorB.get_max_rpm();

            powerA = constrain(powerA*MAX_MOTEUR_POWER, -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);
            powerB = constrain(powerB*MAX_MOTEUR_POWER, -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);

            Serial.print(" PowA: "+String(powerA)+" PowB: "+String(powerB));

            _motorH.set_target_power(powerA);
            _motorB.set_target_power(powerB);

            // CrcLib::SetPwmOutput(CRC_PWM_1, -powerA);
            // CrcLib::SetPwmOutput(CRC_PWM_2, -powerB);
        }

        /**
         * Set the diff angle and spin direction in _moveParam
         * Also determines travel direction
         * @param currentAngle Wheel angle in radians
         * @param _target_angle Desired angle in radians
         */
        void get_diff_angle(double currentAngle){
            //Calculate travel angle, will be negative
            _moveParam.diff = _target_angle - currentAngle;
            if(_moveParam.diff >= 0){
                _moveParam.dir = _moveParam.diff < M_PI ? 
                                _moveParam.diff > M_PI/2 ? Direction::CLOCKWISE:Direction::COUNTERCLOCKWISE 
                                :_moveParam.diff > 3*M_PI/2 ? Direction::CLOCKWISE:Direction::COUNTERCLOCKWISE;
            }
            else if(_moveParam.diff < 0){
                _moveParam.dir = _moveParam.diff > -M_PI ? 
                                _moveParam.diff > -M_PI/2 ? Direction::CLOCKWISE:Direction::COUNTERCLOCKWISE 
                                :_moveParam.diff > -3*M_PI/2 ? Direction::CLOCKWISE:Direction::COUNTERCLOCKWISE;
            }
            
            //Keep value in a cercle
            while(_moveParam.diff > 2*M_PI){
                _moveParam.diff = _moveParam.diff - 2*M_PI;
            }
            while(_moveParam.diff < -2*M_PI){
                _moveParam.diff = _moveParam.diff + 2*M_PI;
            }

        }
        
        /**
         * Get the current wheel angle from the absolute encoder
         * @return angle in rads
         */
        double get_current_angle(){
            //Get encoder value, make sure the value is between 0 and 4160 (Observation de Félix: L'encodeur retour parfois 8000)
            double enco = pulseIn(_abs_enco_pin, HIGH);

            //Calculate angle
            double angleAct = enco/4160.0 * (2*M_PI);

            //Constrain manuel, la fonction fourni bug des fois
            if(angleAct > 2* M_PI){
                angleAct = 2*M_PI;
            }
            else if(angleAct<0){
                angleAct = 0;
            }

            return angleAct;
        }

        /**
         * Get the angular component using a ratio of the difference
         * between current and target angle, round to 0,001
         */
        double get_angular_component(double diff, double speedFactor){
            double component = sin(diff)*speedFactor;
            return component;
        }
        
        /**
         * Get the translation component using a ratio of the difference
         * between current and target angle, round to 0,001
         */
        double get_translation_component(double diff, double speedFactor){
            double component = cos(diff)*speedFactor;
            return component;
        }

        /**
         * Transform the diff angle to be usable be the PID
         */
        double get_PID_angle(double diff){
            if(diff > 0){
                //Keep angle between 0 and M_PI
                if(diff > M_PI){
                    diff -= M_PI;
                }

                //Take shortest angle
                if(diff > M_PI_2){
                    diff = M_PI - diff;
                }
                else{
                    diff = -fabs(diff);
                }

            }
            else if(diff < 0){
                //Keep angle between 0 and M_PI
                if(diff < -M_PI){
                    diff += M_PI;
                }

                //Take shortest angle
                if(diff < -M_PI_2){
                    diff = M_PI + diff;
                }
                else{
                    diff = -fabs(diff);
                }
            }

            return diff;
        }

        /**
         * Set the next position and power of swerve
         * @param _target_angle Desired angle of wheel in rads (0 to 2pi)
         * @param _trans_power Multiplication factor for translation power between -1.0 and 1.0
         */
        void set_target(double tar_angle, double trans_power){
            _target_angle = tar_angle;
            _trans_power = trans_power;
            Serial.print(" TarAng: "+String(_target_angle)+" TransPow: "+String(_trans_power));
        }

    private:
        //-------------------------- VARIABLES ----------------------------
        PID_RT _pid;
        drives::PrecisionMotor _motorH;
        drives::PrecisionMotor _motorB;

        Vec2D _vecPower; //x=trans, y=angular
        TravelParam _moveParam;

        double _target_angle, _trans_power;
        double _abs_enco_pin;
};
