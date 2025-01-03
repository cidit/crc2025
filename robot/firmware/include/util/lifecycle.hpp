
#pragma once

class Lifecycle
{
public:
    virtual ~Lifecycle() = default;
    virtual void begin() = 0;
    virtual void update() = 0;
};
