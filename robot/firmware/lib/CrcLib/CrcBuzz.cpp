#include "CrcBuzz.h"

namespace CrcUtility {
void CrcBuzz::Initialize(unsigned char pin)
{
    _buzzPin = pin;
    pinMode(_buzzPin, OUTPUT);
}

void CrcBuzz::StartTune(Tune* tune)
{
    _currentTune = tune;

    Update(0);
}

void CrcBuzz::Update(unsigned int delta)
{
    if (_currentTune == NULL)
        return;

    if (_currentTune->Update(
            delta,
            [](pitch_t note, void* self) { ((CrcBuzz*)self)->SetTone(note); },
            this))
        _currentTune = NULL;
}

void CrcBuzz::SetTone(unsigned int pitch)
{
    if (pitch == NOTE_SILENCE)
        noTone(_buzzPin);
    else
        tone(_buzzPin, pitch);
}
}
