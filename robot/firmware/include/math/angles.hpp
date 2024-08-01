#pragma once

class Angle
{
public:
    double _radians;
    static Angle from_deg(double degrees)
    {
        // TODO: check bounds, wrap?
        return Angle(degrees/180);
    }
    static Angle from_rad(double rads)
    {
        // TODO: check bounds, wrap?
        return Angle(rads);
    }

private:
    Angle(double radians) : _radians(radians) {}
};

Direction shortest_path(Angle from, Angle to) {
    // TODO: unimplemented
    return Direction::CLOCKWISE; // DUMMY VALUE
}