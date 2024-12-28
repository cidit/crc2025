#include <Arduino.h>
#include "drives/motor.hpp"
#include "util/looped.hpp"

namespace drives
{

    struct PrecisionHinge
    {


        Motor _m;

        /**
         * 
         */
        double _clockwise_stop, _counter_clockwise_stop;

        /**
         * 
         */
        PrecisionHinge(
            Motor m, double c_stop, double cc_stop
        ) : _m(m), _clockwise_stop(c_stop), _counter_clockwise_stop(cc_stop)
        {
            
        }

        /**
         * @param position: exact position as a percentage between counter-clockwise stop and clockwise stop
         */
        void set_position(double position) 
        {
            
        }
    };
}