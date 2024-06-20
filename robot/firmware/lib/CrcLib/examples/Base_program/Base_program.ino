//This is how Arduino IDE loads the CrcLib when compiling the code, allowing the use use its functions.
#include <CrcLib.h>


//The "setup" function is Arduino-specific. All the lines in the brackets right after it will be executed only once, when the CrcDuino is powered up.
void setup()
{
    //This call must be made in order to properly set up everything in order for CrcLib functions to be executed properly.
    CrcLib::Initialize();
    Serial.begin(9600);  //Establishes a 9600 baud rate connection with the monitor.
}



//The “loop” function is Arduino-specific. All the lines in the brackets right after it will be executed cyclically as long as the CrcDuino is powered up.
void loop()
{
    //This call must be made in order to periodically update CrcLib data, including the remote controller state.
    CrcLib::Update();
}

//Lines starting with 2 slashes won't be compiled; they are comments.
/*This is a way 
to comment multiple lines of code */
