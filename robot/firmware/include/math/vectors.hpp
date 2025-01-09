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

    T norm()
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

class Vec2D : public Vector<double, 2>
{
public:
    Vec2D()
    {
        components[0] = 0;
        components[1] = 0;
    }
    Vec2D(double x, double y)
    {
        components[0] = x;
        components[1] = y;
    }
    double x()
    {
        return components[0];
    }
    void set_x(double x)
    {
        components[0] = x;
    }
    double y()
    {
        return components[1];
    }
    void set_y(double y)
    {
        components[1] = y;
    }

    /**
     * gets the angle value of the vec between 0 and 2pi
     */
    double angle()
    {
        // double angle = atan(components[1] / components[0]);
        // return components[0] < 0 ? 180 + angle : components[1] > 0 ? angle
        //                                                            : 360 + angle;

        const auto angle_on_zero = atan2(components[1], components[2]);
        if (angle_on_zero >= 0) {
            return angle_on_zero;
        }
        return (2* M_PI) + angle_on_zero;
    }

    /**
     * TODO: this function is a shitshow. entirely rewrite.
     */
    Vec2D normalize()
    {
        auto norm = this->norm();

        for (auto i = 0; i < 2; i++)
        {
            components[i] = components[i] / norm;
        }

        return Vec2D(components[0], components[1]);
    }

    static Vec2D from_polar(double rads, double length) {
        return Vec2D(
            length*cos(rads),
            length*sin(rads)
        );
    }
};