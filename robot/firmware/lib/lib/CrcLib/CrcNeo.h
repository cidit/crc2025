#ifndef CrcNeo_h
#define CrcNeo_h

#include "CrcUtils.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

namespace CrcUtility {
struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;

    constexpr Color(unsigned char red, unsigned char green, unsigned blue)
        : r(red)
        , g(green)
        , b(blue)
    {
    }

    constexpr Color()
        : r(0)
        , g(0)
        , b(0)
    {
    }
};

const Color NO_COLOR = { 0, 0, 0 };

const Color RED_HIGH     = { 255, 0, 0 };
const Color GREEN_HIGH   = { 0, 255, 0 };
const Color BLUE_HIGH    = { 0, 0, 255 };
const Color YELLOW_HIGH  = { 255, 255, 0 };
const Color CYAN_HIGH    = { 0, 255, 255 };
const Color MAGENTA_HIGH = { 255, 0, 255 };
const Color WHITE_HIGH   = { 255, 255, 255 };

const Color ORANGE_HIGH = { 255, 128, 0 };

const Color RED_LOW     = { 25, 0, 0 };
const Color GREEN_LOW   = { 0, 25, 0 };
const Color BLUE_LOW    = { 0, 0, 25 };
const Color YELLOW_LOW  = { 25, 25, 0 };
const Color CYAN_LOW    = { 0, 25, 25 };
const Color MAGENTA_LOW = { 25, 0, 25 };
const Color WHITE_LOW   = { 25, 25, 25 };

using ColorDuration      = PatternItem<Color>;
using ColorPattern       = Pattern<Color>;
using SimpleColorPattern = SimplePattern<Color>;

const ColorDuration PATTERN_TEST[] = { { 250, RED_LOW }, { 250, NO_COLOR },
    { 250, RED_LOW }, { 250, NO_COLOR }, { 250, RED_LOW }, { 500, NO_COLOR },
    { 750, GREEN_LOW }, { 250, NO_COLOR }, { 750, GREEN_LOW },
    { 250, NO_COLOR }, { 750, GREEN_LOW }, { 500, NO_COLOR }, { 250, BLUE_LOW },
    { 250, NO_COLOR }, { 250, BLUE_LOW }, { 250, NO_COLOR }, { 250, BLUE_LOW },
    { 500, NO_COLOR }, ColorDuration::END };

const ColorDuration PATTERN_FASTCHRISTMAS[]
    = { { 100, RED_LOW }, { 100, GREEN_LOW }, ColorDuration::END };

const ColorDuration PATTERN_BLINDINGDANGER[] = { { 200, YELLOW_HIGH },
    { 500, NO_COLOR }, { 100, YELLOW_HIGH }, { 100, NO_COLOR },
    { 100, YELLOW_HIGH }, { 500, NO_COLOR }, ColorDuration::END };

const ColorDuration PATTERN_STARTUP[]
    = { { 500, RED_HIGH }, { 500, GREEN_HIGH }, { 500, BLUE_HIGH },
          { 500, YELLOW_HIGH }, { 500, CYAN_HIGH }, { 500, MAGENTA_HIGH },
          { 500, WHITE_HIGH }, ColorDuration::END };

const ColorDuration PATTERN_CONNECTED[] = { { 100, CYAN_HIGH },
    { 100, NO_COLOR }, { 100, CYAN_HIGH }, { 100, NO_COLOR },
    { 100, CYAN_HIGH }, { 500, NO_COLOR }, ColorDuration::END };

const ColorDuration PATTERN_DISCONNECTED[] = { { 100, ORANGE_HIGH },
    { 100, NO_COLOR }, { 100, ORANGE_HIGH }, { 100, NO_COLOR },
    { 100, ORANGE_HIGH }, { 500, NO_COLOR }, ColorDuration::END };

const ColorDuration PATTERN_ERROR1[] = { { 150, RED_LOW }, { 150, NO_COLOR },
    { 150, RED_LOW }, { 150, NO_COLOR }, { 400, YELLOW_LOW },
    { 2000, NO_COLOR }, ColorDuration::END };

const ColorDuration PATTERN_ERROR2[] = { { 150, MAGENTA_LOW },
    { 150, NO_COLOR }, { 150, CYAN_LOW }, { 150, NO_COLOR },
    { 150, MAGENTA_LOW }, { 2000, NO_COLOR }, ColorDuration::END };

const ColorDuration PATTERN_ERROR3[] = { { 400, CYAN_LOW }, { 150, NO_COLOR },
    { 400, RED_LOW }, { 2000, NO_COLOR }, ColorDuration::END };

const ColorDuration PATTERN_ERROR4[] = { { 150, YELLOW_LOW }, { 150, NO_COLOR },
    { 400, MAGENTA_LOW }, { 2000, NO_COLOR }, ColorDuration::END };

class CrcNeo {
public:
    CrcNeo(unsigned char pin)
        : _neoPin(pin)
        , _pixel(Adafruit_NeoPixel(1, pin, NEO_GRB + NEO_KHZ800))
    {
    }
    ~CrcNeo() = default;

    void Initialize();

    void Update(unsigned int delta);

    void SetColor(Color col);

    void StartPattern(ColorPattern* pattern);

private:
    ColorPattern* _currentPattern = NULL;

    unsigned char _neoPin;

    Adafruit_NeoPixel _pixel;
};
}
#endif
