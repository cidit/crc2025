#pragma once
#include <cmath>

namespace math
{

    const double MAX_RAD = PI * 2;

    class Angle
    {
    public:
        double _radians;

        static Angle from_deg(double degrees)
        {
            return from_ratio(degrees / 360);
        }

        static Angle from_rad(double rads)
        {
            double bounds_checked = fmod(rads, MAX_RAD);
            auto positive_angle = bounds_checked < 0
                                      ? MAX_RAD - bounds_checked
                                      : bounds_checked;
            return Angle(positive_angle);
        }

        static Angle from_ratio(double ratio)
        {
            return Angle::from_rad(ratio * MAX_RAD);
        }

        static Angle zero() {
            return Angle(0);
        }

        Angle(const Angle &other) : _radians(other._radians) {};

        /**
         * gets the opposite of the angle
         * ex: 30deg -> 210deg
         */
        Angle operator-()
        {
            return from_rad(_radians - MAX_RAD);
        }

        /**
         * parallel reflection of the angle
         * ex: 30deg -> 330deg
         */
        Angle parallel()
        {
            return from_rad(-_radians);
        }

        double ratio() {
            return _radians / MAX_RAD;
        }

    private:
        Angle(double radians) : _radians(radians) {}
    };

    Direction shortest_direction(Angle from, Angle to)
    {
        auto zeroed = Angle::from_rad(from._radians - to._radians);
        return zeroed._radians < PI ? CLOCKWISE : COUNTER_CLOCKWISE;
    }

}