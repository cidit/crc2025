#include <SPI.h>
#include <Servo.h>
#include "PID_RT.h"


#define CS_SPI_PIN 10

int pins[] = {9, 6};
const int arraySize = sizeof(pins) / sizeof(int);
Servo myservo[arraySize];

typedef struct mystruct
{
    uint8_t JoyLeft_X;
    uint8_t JoyLeft_Y;
    // uint8_t JoyRight_X;
    // uint8_t JoyRight_Y;
    // bool boutton_A;
    // bool boutton_B;
    // bool boutton_X;
    // bool boutton_Y;
    // bool boutton_UP;
    // bool boutton_DOWN;
    // bool boutton_RIGHT;
    // bool boutton_LEFT;
};

typedef union patate
{
    mystruct spiMsg;
    byte table[sizeof(mystruct)];
};

patate msg;

PID_RT PID_distanceController;
PID_RT PID_angleController;

double targetrad = 0;
volatile bool received = false;

double angleOptimisation(double radians)
{
    if (radians > PI())
    {
        radians -= 2 * PI();
        if (radians > -PI())
        {
            radians += PI();
        }
    }
    if (radians < -PI())
    {

        radians += 2 * PI();
        if (radians > -PI())
        {
            radians += PI();
        }
    }
    return radians;
}

double cartToPolAngle(int x, int y)
{

    double polVectorAngle;
    polVectorAngle = atan2(y, x);

    if (y == 1 && x == 0)
    {
        polVectorAngle = 0; // make other thing to repr no angle
        return polVectorAngle;
    }
    if (x == 0 && y > 0)
    {
        polVectorAngle = PI / 2;
        return polVectorAngle;
    }
    else if (x == 0 && y < 1)
    {
        polVectorAngle = 3 * PI / 2;
        return polVectorAngle;
    }
    else if (x > 0 && y == 1)
    {
        polVectorAngle = 0;
        return polVectorAngle;
    }
    else if (x < 0 && y == 1)
    {
        polVectorAngle = PI;
        return polVectorAngle;
    }

    // return polVectorAngle;

    if (x < 0 && y < 1)
    {
        polVectorAngle = atan2(y, x) + (2 * PI);
        return polVectorAngle;
    }
    else if (x > 0 && y < 1)
    {
        polVectorAngle = (atan2(y, x)) + 2 * PI;
        return polVectorAngle;
    }
    else
    {
        return polVectorAngle;
    }
}

doube cartToPolNorm(float x, float y)
{
    return sqrt(sq(x) + sq(y));
}


void moveSwerve(double targetX, double targetY,){ // angle from angleOptimisation
    
    
    
    
    //TODO:: angle to RPM for PID
    /*PID.setPoint(angle); angle from controller

    if(PID.compute(measAngle)){ meas from encoder


    int PWMvalueY = PID.getOutput() * 5 + 1500;  // negative ? 
    int PWMvalueX = PID.getOutput() * 5 + 1500;

    myservo[0].writeMicroseconds(PWMvalueX);
    myservo[1].writeMicroseconds(PWMvalueY);
    } */
}
ISR(SPI_STC_vect)
{
    static byte index = 0;
    byte receivedByte = SPDR;
    msg.table[index++] = receivedByte;
    if (index == sizeof(mystruct))
    {
        index = 0;
        received = true;
    }
}



void setup()
{

    Serial.begin(115200);
    pinMode(CS_SPI_PIN, OUTPUT);
    pinMode(MISO, OUTPUT); // Configure MISO en sortie (pour envoyer des données au contrôleur)
    digitalWrite(CS_SPI_PIN, HIGH);
    SPCR |= _BV(SPE);      // Configure l'interface SPI comme étant un périphérique
    SPI.attachInterrupt(); // Permet les interruptions sur la réception de données SPI
    for (int i = 0; i < 2; i++)
        myservo[i].attach(pins[i]);
    pinMode(pins, OUTPUT);
}

void loop()
{
    if (received)
    {
        Serial.print("JoyLeft_X: ");
        Serial.println(msg.spiMsg.JoyLeft_X);
        Serial.print("JoyLeft_Y: ");
        Serial.println(msg.spiMsg.JoyLeft_Y);
        received = false;
    }
    int joyL_X = map(msg.spiMsg.JoyLeft_X, 0, 255, -100, 100);
    int joyL_Y = map(msg.spiMsg.JoyLeft_Y, 0, 255, -100, 100);
    int PWMvalueY = joyL_Y * 5 + 1500;
    int PWMvalueX = joyL_X * 5 + 1500;
    myservo[0].writeMicroseconds(PWMvalueX);
    myservo[1].writeMicroseconds(PWMvalueY);

    // float cartToPolJoyL_X = map(msg.spiMsg.JoyLeft_X, 0, 255, -1, 1);
    // float cartToPolJoyL_Y = map(msg.spiMsg.JoyLeft_Y, 0, 255, -1, 1);

    double calcPolAngle = cartToPolangle(joyL_X, joyL_Y);
    double calcPolNorm = cartToPolNorm(joyL_X, joyL_Y);

    int PWMvalueY = joyL_Y * 5 + 1500;
    int PWMvalueX = joyL_X * 5 + 1500;

    myservo[0].writeMicroseconds(PWMvalueX);
    myservo[1].writeMicroseconds(PWMvalueY);
    // myservo[2].writeMicroseconds(0);
    // myservo[1].writeMicroseconds(0);
}
