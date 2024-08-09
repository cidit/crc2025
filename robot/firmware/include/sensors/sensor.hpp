#pragma once

namespace sensors
{

    template <typename Measure>
    class Sensor
    {

    protected:
        Measure _last;
        Sensor(Measure last_default_val): _last(last_default_val) {}

    public:
        virtual void begin() = 0;
        virtual bool sample(Measure &out) = 0;
        virtual ~Sensor() = default;

        void poll()
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
    };

} // namespace sensors