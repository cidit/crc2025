#include "CrcNeo.h"

namespace CrcUtility {

void CrcNeo::Initialize()
{
    pinMode(_neoPin, OUTPUT);

    _pixel.begin();
    _pixel.show();
}

void CrcNeo::SetColor(Color color)
{
    _pixel.setPixelColor(0, color.r, color.g, color.b);
    _pixel.show();
}

void CrcNeo::StartPattern(ColorPattern* pattern)
{
    _currentPattern = pattern;
    Update(0);
}

void CrcNeo::Update(unsigned int delta)
{

    if (_currentPattern == NULL)
        return;

    if (_currentPattern->Update(
            delta, [](Color c, void* self) { ((CrcNeo*)self)->SetColor(c); },
            this))
        _currentPattern = NULL;
}
}
