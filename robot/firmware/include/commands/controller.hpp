#pragma once
#include "math/vectors.hpp"
#include "CrcRemoteState.h"
#include "CrcLib.h"

/**
 * largely based off of @squid's controller
 */
class Controller
{
public:
    /**
     * this struct's angle field gets updated if it's
     * raw values' norm is 0.
     */
    struct joystick
    {
        Vec2D raw;
        double angle;
    } j1, j2;
    
    CrcUtility::RemoteState _raw_last, raw_current;
};