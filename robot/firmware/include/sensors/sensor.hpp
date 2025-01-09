#pragma once
#include "util/lifecycle.hpp"
#include "util/timer.hpp"

template <typename Measure>
class Sensor : public Lifecycle
{
protected:
    Measure _last;
    const Timer &_polling_timer;
    Sensor(Measure last_default_val, const Timer &polling_timer)
        : _last(last_default_val),
          _polling_timer(polling_timer) {}

public:
    virtual void begin() = 0;
    virtual bool sample(Measure &out) = 0;
    virtual ~Sensor() = default;

    virtual void poll()
    {
        Measure measure;
        auto success = sample(measure);
        if (success)
        {
            _last = measure;
        }
    }

    Measure getLast()
    {
        return _last;
    }

    void update() override
    {
        if (_polling_timer.is_time())
        {
            Serial.print("update");
            this->poll();
        }
    }
};
