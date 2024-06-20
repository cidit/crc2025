#include "sensors/encoder.hpp"
#include <Encoder.h>

EncoderSensor::EncoderSensor(pin_t clock, pin_t clock_offset) 
    : _internal_encoder(Encoder(clock, clock_offset))
{
    
}

bool EncoderSensor::sample(EncoderData &out)
{
    return false;
}