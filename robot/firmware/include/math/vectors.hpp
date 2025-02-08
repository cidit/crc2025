#pragma once

#include <Arduino.h>
#include <math.h>

template <typename T, size_t D>
class Vector
{
public:
    T components[D];

    // TODO: come back to that fucking shit
    // Vector(): components {0, 0} {}
    // Vector(T components[D]): components(components) {}

    const T norm()
    {
        T total = 0;
        for (T t : components)
        {
            total += pow(t, 2);
        }
        return sqrt(total);
    }

    Vector<T, D> normalize()
    {
        auto norm = this->norm();

        for (auto i = 0; i < D; i++)
        {
            components[i] = components[i] / norm;
        }
    }

    // static Vector rotate(const Angle &angle)
    // {
    //     auto len = norm();
    //     // TODO: unimplemented
    // }

    static Vector<T, D> zero()
    {
        return Vector<T, D>{{0}};
    }
};

using Vec3D = Vector<double, 3>;

class Vec2D
{
public:
    double _x, _y;
    Vec2D()
    {
        _x = _y = 0;
    }
    Vec2D(double x, double y)
    {
        _x = x;
        _y = y;
    }
    double x() const 
    {
        return _x;
    }
    void set_x(double x)
    {
        _x = x;
    }
    double y() const
    {
        return _y;
    }
    void set_y(double y)
    {
        _y = y;
    }

    /**
     * gets the angle value of the vec between 0 and 2pi
     */
    double angle() const
    {
        // double angle = atan(components[1] / components[0]);
        // return components[0] < 0 ? 180 + angle : components[1] > 0 ? angle
        //                                                            : 360 + angle;

        const auto angle_on_zero = atan2(_y, _x);
        if (angle_on_zero >= 0) {
            return angle_on_zero;
        }
        return (2* M_PI) + angle_on_zero;
    }

    double norm() {
        return sqrt(pow(_x, 2) + pow(_y, 2));
    }

    /**
     * TODO: this function is a shitshow. entirely rewrite.
     */
    Vec2D normalize()
    {
        auto norm = this->norm();
        return Vec2D(_x/norm, _y/norm);
    }

    static Vec2D from_polar(double rads, double length) {
        return Vec2D(
            length*cos(rads),
            length*sin(rads)
        );
    }

    Vec2D rotate_by(double rads) {
        double cosTheta = cos(rads);
        double sinTheta = sin(rads);

        double newX = _x * cosTheta - _y * sinTheta;
        double newY = _x * sinTheta + _y * cosTheta;

        return Vec2D(newX, newY);
    }
};