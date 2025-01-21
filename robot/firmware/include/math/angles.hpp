#pragma once
#include "util/misc.hpp"

static const double MAX_RAD = PI * 2;

class Angle : public Printable
{
public:
    double _radians;

    Angle(double radians = 0) : _radians(radians) {}

    virtual size_t printTo(Print &p) const override
    {
        p.print(_radians);
        return sizeof(_radians);
    }

    static Angle from_deg(double degrees)
    {
        return from_ratio(degrees / 360);
    }

    static Angle from_rad(double rads)
    {
        double bounds_checked = fmod(rads, MAX_RAD);
        auto positive_angle = bounds_checked < 0
                                  ? MAX_RAD + bounds_checked
                                  : bounds_checked;
        return Angle(positive_angle);
    }

    static Angle from_ratio(double ratio)
    {
        return Angle::from_rad(ratio * MAX_RAD);
    }

    static Angle zero()
    {
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

    double ratio()
    {
        return _radians / MAX_RAD;
    }

    /**
     * finds the shortest travel from the source to the destination
     */
    static double travel(const Angle &source, const Angle &destination)
    {
        auto zeroed = destination._radians - source._radians;
        if (zeroed > PI)
        {
            return zeroed - MAX_RAD;
        }
        else if (zeroed < -PI)
        {
            return zeroed + MAX_RAD;
        }
        else
        {
            return zeroed;
        }
    }

    /**
     * Reinterprets this object in the range [-PI, PI]
     */
    double zeroed()
    {
        return travel(*this, Angle::zero());
    }
};
