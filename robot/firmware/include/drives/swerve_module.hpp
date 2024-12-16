#pragma once

#include "math.h"
#include "math/vectors.hpp"
#include <PID_RT.h>
using math::cartesian::Vec2D;
#include "drives/precision_motor.hpp"
#include <CrcLib.h>

class SwerveModule
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
        SwerveModule(drives::PrecisionMotor motorH, drives::PrecisionMotor motorB)
            : _motorH(motorH),
            _motorB(motorB)
        {

        }
        /**
         * Initialise the default values for the calculations
         * Must be call in Setup()
         */
        void init()
        {
        }

        //-------------------------- PUBLICS -----------------------------
        /**
         * Calculate the power vector (translation, angular) of a Swerve module
         * and set the power of the motors accorddingly
         * @param targetAngle Desired angle of wheel in rads (0 to 2pi)
         * @param tPower Multiplication factor for translation power between -1.0 and 1.0
         * @return done - True: Operation completed succesfully, False: Problem
         */
        void loop(double targetAngle, double tPower){
            Vec2D vector = calculate(targetAngle, tPower);
            set_motor_powers(vector);
        }

        /**
         * Calculate the power vector (translation, angular) of a Swerve module
         * @param targetAngle Desired angle of wheel in rads (0 to 2pi)
         * @param tPower Multiplication factor for translation power between -1.0 and 1.0
         * @return vecPower - {x=translationComponent, y=angularComponent}
         */
        Vec2D calculate(double targetAngle, double tPower){
            Serial.println(tPower);

            //Set diff and dir in _moveParam
            double currentAngle = get_current_angle();
            get_diff_angle(currentAngle, targetAngle);

            //Find the absolute components of the power vector of the entire module
            if(_pid.compute(get_PID_angle(_moveParam.diff)) && tPower != 0){
                _vecPower.set_y(fabs(_pid.getOutput()));
                _vecPower.set_x(get_translation_component(fabs(_moveParam.diff), fabs(tPower)));
            }
            else if(tPower == 0){ //If the joy is not in use
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
            if(targetAngle <= M_PI){
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

            powerA = constrain(powerA*MAX_MOTEUR_POWER, -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);
            powerB = constrain(powerB*MAX_MOTEUR_POWER, -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);

            CrcLib::SetPwmOutput(CRC_PWM_1, -powerA);
            CrcLib::SetPwmOutput(CRC_PWM_2, -powerB);
        }

        /**
         * Set the diff angle and spin direction in _moveParam
         * Also determines travel direction
         * @param currentAngle Wheel angle in radians
         * @param targetAngle Desired angle i radians
         */
        void get_diff_angle(double currentAngle, double targetAngle){
            //Calculate travel angle, will be negative
            _moveParam.diff = targetAngle - currentAngle;
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
            double enco = pulseIn(CRC_PWM_12, HIGH);

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
    
    private:
        //-------------------------- VARIABLES ----------------------------
        PID_RT _pid;
        drives::PrecisionMotor _motorH;
        drives::PrecisionMotor _motorB;

        Vec2D _vecPower; //x=trans, y=angular
        TravelParam _moveParam;
};
