#pragma once

#include "math/vectors.hpp"
using math::cartesian::Vec2D;

class Controller{

    public:
        Controller();
        Vec2D get_translation_vec();
        Vec2D get_rotation_vec();
};