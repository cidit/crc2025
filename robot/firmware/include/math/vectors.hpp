#pragma once

#include <Arduino.h>
#include <cmath>

namespace math
{

    template <typename T, size_t D>
    class Vector
    {
    public:
        T components[D];

        T norm()
        {
            T total = 0;
            for (T t : components)
            {
                total += pow(t, 2);
            }
            return sqrt(total);
        }

        static Vector rotate(const Angle &angle)
        {
            auto len = norm();
            // TODO: unimplemented
        }

        static Vector<T, D> zero()
        {
            return Vector<T, D>{{0}};
        }
    };

    namespace cartesian
    {
        using Vec3D = Vector<double, 3>;
    }
}