#include <Encoder.h>
#include "cmath"
#include "sensors/encoder.hpp"

void RotaryEncoder::begin() {

}

bool RotaryEncoder::sample(RotaryEncoderData &out)
{
    auto internal_counter = _internal_encoder.read();
    if (internal_counter > 0) {
        auto a = std::fmod(float(internal_counter), _ticks_per_rotation);
    }
    return false;
}