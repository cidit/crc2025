// Timers
// https://robocrc.atlassian.net/wiki/spaces/AR/pages/761200650/Timer

// Print the remote payload
// https://robocrc.atlassian.net/wiki/spaces/AR/pages/761200641/PrintControllerState
#include <CrcLib.h>

CrcLib::Timer timer;
void setup() {
    CrcLib::Initialize();
    Serial.begin(115200);
    Serial.println("Debug started");

    // Start the timer with a period of 10 ms
    timer.Start(10);

    /*
    The rest of your setup code
    ...
     */
}

void loop() {
    CrcLib::Update();

    if (timer.IsFinished()) { // 10 ms has elapsed
        timer.Next(); // Start the next period of the timer

        // Then perform the rest of your logic
        if (CrcLib::IsCommValid()) {
            CrcLib::PrintControllerState(); // Print the state of the controller if connected
        } else {
            Serial.println("Controller not connected");
        }
    }

    /*
    The rest of your looping code
    ...
     */
}
