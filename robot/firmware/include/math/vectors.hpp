#pragma once

#include <Arduino.h>

namespace math
{

    template <typename T, size_t D>
    struct Vector
    {
        T components[D];
    };

    namespace cartesian
    {
        using Vec3D = Vector<double, 3>;
    }
}