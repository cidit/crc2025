#pragma once
#include "math/vectors.hpp"
#include "util/lifecycle.hpp"
#include "CrcRemoteState.h"
#include "CrcLib.h"
#include "CrcXbee.h"

/**
 * largely based off of @squid's controller
 */
class Controller: public Lifecycle
{
public:
    /**
     * this struct's angle field gets updated if it's
     * raw values' norm is 0.
     */
    struct joystick
    {
        Vec2D xy;
        double angle;
    } j1, j2;

    CrcUtility::RemoteState _raw_last, _raw_current;
    
    void begin() override {

    }

    void update() override {
    
    }
};