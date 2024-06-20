// Comment this when trying to use intellisense in visual micro
#include <CrcLib.h>
//------------------------------------------------------------

// Define some color pattern using { <duration in ms>, <color> }
ColorDuration pattern[]
    = { { 750, Color(213, 4, 200) }, { 500, BLUE_LOW }, ColorDuration::END };
// And some song using { <duration in ms>, <note frequency in Hz> }
Note notes[] = { { 250, NOTE_B4 }, { 500, NOTE_SILENCE }, Note::END };

// Some more patterns
ColorDuration pattern_error[] = { { 250, Color(213, 4, 30) }, { 250, NO_COLOR },
    { 1000, BLUE_LOW }, ColorDuration::END };
Note notes_error[] = { { 250, NOTE_B4 }, { 500, NOTE_F5 }, Note::END };

void setup() { CrcLib::Initialize(); }

void loop()
{
    static bool started = false;

    CrcLib::Update();
    if (millis() > 3000 && !started) {
        // After 3s, play a tune once and show a looping pattern
        CrcLib::PlayTune(notes, false);
        CrcLib::ShowColorPattern(pattern, true);
        started = true;
    }
    if (millis() > 5000) {
        // Stop the program and show the error light and sound pattern
        CrcLib::StopEverythingFromError(notes_error, pattern_error);
    }
}