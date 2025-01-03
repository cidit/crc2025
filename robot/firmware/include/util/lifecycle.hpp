
#pragma once

class Lifecycle
{
    virtual ~Lifecycle() = default;
    virtual void begin() = 0;
    virtual void update() = 0;
};
