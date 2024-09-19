#pragma once

class Looped
{
public:
    virtual ~Looped() = default;

    /**
     * represents a loop step.
     */
    virtual void loop() = 0;
};