#include "CrcRemoteState.h"

namespace CrcUtility {
uint8_t RemoteState::serialize_update(
    uint8_t* buf, bool keyframe, RemoteState& prev)
{
    uint8_t len = 1;
    if (prev.gachetteG != gachetteG || keyframe) {
        buf[0] |= 0x01;
        buf[len++] = gachetteG;
    }
    if (prev.gachetteD != gachetteD || keyframe) {
        buf[0] |= 0x02;
        buf[len++] = gachetteD;
    }
    if (prev.joystick1X != joystick1X || prev.joystick1Y != joystick1Y
        || keyframe) {
        buf[0] |= 0x04;
        buf[len++] = joystick1X;
        buf[len++] = joystick1Y;
    }
    if (prev.joystick2X != joystick2X || prev.joystick2Y != joystick2Y
        || keyframe) {
        buf[0] |= 0x08;
        buf[len++] = joystick2X;
        buf[len++] = joystick2Y;
    }
    uint8_t prev_but1 = prev.serialize_button_bank1();
    uint8_t but1      = serialize_button_bank1();
    if (prev_but1 != but1 || keyframe) {
        buf[0] |= 0x10;
        buf[len++] = but1;
    }
    uint8_t prev_but2 = prev.serialize_button_bank2();
    uint8_t but2      = serialize_button_bank2();
    if (prev_but2 != but2 || keyframe) {
        buf[0] |= 0x20;
        buf[len++] = but2;
    }
    return len;
}

uint8_t RemoteState::serialize_button_bank1()
{
    uint8_t val = 0;
    val |= arrowRight;
    val |= arrowUp << 1;
    val |= arrowLeft << 2;
    val |= arrowDown << 3;
    val |= colorRight << 4;
    val |= colorUp << 5;
    val |= colorLeft << 6;
    val |= colorDown << 7;
    return val;
}

uint8_t RemoteState::serialize_button_bank2()
{
    uint8_t val = 0;
    val |= L1;
    val |= R1 << 1;
    val |= select << 2;
    val |= start << 3;
    val |= logo << 4;
    val |= hatL << 5;
    val |= hatR << 6;
    return val;
}

void RemoteState::deserialize_update(uint8_t* payload)
{
    uint8_t len = 0;
    if (payload[0] & 0x01) {
        gachetteG = payload[++len];
    }
    if (payload[0] & 0x02) {
        gachetteD = payload[++len];
    }
    if (payload[0] & 0x04) {
        joystick1X = payload[++len];
        joystick1Y = payload[++len];
    }
    if (payload[0] & 0x08) {
        joystick2X = payload[++len];
        joystick2Y = payload[++len];
    }
    if (payload[0] & 0x10) {
        uint8_t b  = payload[++len];
        arrowRight = b & 1;
        arrowUp    = (b >> 1) & 1;
        arrowLeft  = (b >> 2) & 1;
        arrowDown  = (b >> 3) & 1;
        colorRight = (b >> 4) & 1;
        colorUp    = (b >> 5) & 1;
        colorLeft  = (b >> 6) & 1;
        colorDown  = (b >> 7) & 1;
    }
    if (payload[0] & 0x20) {
        uint8_t b = payload[++len];
        L1        = b & 1;
        R1        = (b >> 1) & 1;
        select    = (b >> 2) & 1;
        start     = (b >> 3) & 1;
        logo      = (b >> 4) & 1;
        hatL      = (b >> 5) & 1;
        hatR      = (b >> 6) & 1;
    }
}

static void printPadded(uint8_t val)
{
    int8_t transformedVal = val - 128;
    if (transformedVal >= 0)
        PRINT(" ");
    if (transformedVal < 100)
        PRINT(" ");
    if (transformedVal < 10)
        PRINT(" ");
    PRINT(transformedVal);
}

void RemoteState::PrintPayload()
{
    PRINT("Arrows: ");
    PRINT(arrowUp ? "U" : " ");
    PRINT(arrowRight ? "R" : " ");
    PRINT(arrowDown ? "D" : " ");
    PRINT(arrowLeft ? "L" : " ");
    PRINT(", colors: ");
    PRINT(colorUp ? "U" : " ");
    PRINT(colorRight ? "R" : " ");
    PRINT(colorDown ? "D" : " ");
    PRINT(colorLeft ? "L" : " ");
    PRINT(", L1: ");
    PRINT(L1);
    PRINT(", R1: ");
    PRINT(R1);
    PRINT(", select: ");
    PRINT(select);
    PRINT(", start: ");
    PRINT(start);
    PRINT(", logo: ");
    PRINT(logo);
    PRINT(", hatL: ");
    PRINT(hatL);
    PRINT(", hatR: ");
    PRINT(hatR);
    PRINT(", left trig: ");
    printPadded(gachetteG);
    PRINT(", right trig: ");
    printPadded(gachetteD);
    PRINT(", left X: ");
    printPadded(joystick1X);
    PRINT(", left Y: ");
    printPadded(joystick1Y);
    PRINT(", right X: ");
    printPadded(joystick2X);
    PRINT(", right Y: ");
    printPadded(joystick2Y);
    PRINT("\n");
}
}
