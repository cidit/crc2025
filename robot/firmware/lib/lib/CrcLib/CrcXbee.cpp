#include "CrcXbee.h"

#define BASE_BAUD 9600
#define FAST_BAUD 115200
#define ESP_BAUD 1000000

#define XBEE_115200_BAUD 0x07

#define SPECIAL_OP 0x80
const uint8_t SPECIAL_OP_RESET = SPECIAL_OP | 0x00;

#define ESP_DH 0xC52CC25C

const uint8_t xbee_baud_rate[] = { XBEE_115200_BAUD };
const uint8_t esp_baud_rate[]
    = { (ESP_BAUD >> 16) & 0xFF, (ESP_BAUD >> 8) & 0xFF, ESP_BAUD & 0xFF };

namespace CrcUtility {

bool CrcXbee::ReadDigitalChannel(BUTTON channel) const
{
    switch (channel) {
    case BUTTON::ARROW_RIGHT: return _state.arrowRight;
    case BUTTON::ARROW_UP: return _state.arrowUp;
    case BUTTON::ARROW_LEFT: return _state.arrowLeft;
    case BUTTON::ARROW_DOWN: return _state.arrowDown;
    case BUTTON::COLORS_RIGHT: return _state.colorRight;
    case BUTTON::COLORS_UP: return _state.colorUp;
    case BUTTON::COLORS_LEFT: return _state.colorLeft;
    case BUTTON::COLORS_DOWN: return _state.colorDown;
    case BUTTON::L1: return _state.L1;
    case BUTTON::R1: return _state.R1;
    case BUTTON::SELECT: return _state.select;
    case BUTTON::START: return _state.start;
    case BUTTON::LOGO: return _state.logo;
    case BUTTON::HATL: return _state.hatL;
    case BUTTON::HATR: return _state.hatR;
    case BUTTON::NONE: return 0;
    }
}

int8_t CrcXbee::ReadAnalogChannel(ANALOG channel) const
{
    switch (channel) {
    case ANALOG::JOYSTICK1_X: return _state.joystick1X;
    case ANALOG::JOYSTICK1_Y: return _state.joystick1Y;
    case ANALOG::JOYSTICK2_X: return _state.joystick2X;
    case ANALOG::JOYSTICK2_Y: return _state.joystick2Y;
    case ANALOG::GACHETTE_R: return _state.gachetteD;
    case ANALOG::GACHETTE_L: return _state.gachetteG;
    case ANALOG::NONE: return 0;
    }
}

void CrcXbee::Initialize(HardwareSerial& serial)
{
    serial.begin(BASE_BAUD);
    _xbee.setSerial(serial);
    _serial = &serial;
    HandleInit(NULL);
}

void CrcXbee::HandleInit(AtCommandResponse* rx)
{
    switch (_addrInitState) {
    case READY: return;
    case BEGIN: _addrInitState = WAITING_DH; break;
    case WAITING_DH: {
        if (rx && memcmp(rx->getCommand(), "DH", 2) == 0 && rx->isOk()) {
            uint32_t val = ((uint32_t)rx->getValue()[0] << 24)
                | ((uint32_t)rx->getValue()[1] << 16)
                | ((uint32_t)rx->getValue()[2] << 8)
                | ((uint32_t)rx->getValue()[3]);
            _addr.setMsb(val);
            _isXBee        = val == ESP_DH;
            _addrInitState = WAITING_DL;
        } else {
            // error
        }
        break;
    }
    case WAITING_DL: {
        if (rx && memcmp(rx->getCommand(), "DL", 2) == 0 && rx->isOk()) {
            uint32_t val = ((uint32_t)rx->getValue()[0] << 24)
                | ((uint32_t)rx->getValue()[1] << 16)
                | ((uint32_t)rx->getValue()[2] << 8)
                | ((uint32_t)rx->getValue()[3]);
            _addr.setLsb(val);
            _addrInitState = WAITING_BD;
        } else {
            // error
        }
        break;
    }
    case WAITING_BD: {
        if (rx && memcmp(rx->getCommand(), "BD", 2) == 0 && rx->isOk()) {
            /* Upgrade serial baud rate */
            _serial->end();
            _serial->begin(_isXBee ? FAST_BAUD : ESP_BAUD);
            _addrInitState = READY;
        } else {
            // error
        }
        break;
    }
    }

    // Next, send request
    AtCommandRequest req;
    switch (_addrInitState) {
    case READY: return;
    case BEGIN:
        // TODO: panic
        return;
    case WAITING_DH: req = AtCommandRequest((uint8_t*)"DH"); break;
    case WAITING_DL: req = AtCommandRequest((uint8_t*)"DL"); break;
    case WAITING_BD: {
        const uint8_t* baud = _isXBee ? xbee_baud_rate : esp_baud_rate;
        uint8_t baud_len
            = _isXBee ? sizeof(xbee_baud_rate) : sizeof(esp_baud_rate);
        req = AtCommandRequest(
            (uint8_t*)"BD", (uint8_t*)baud, sizeof(baud_len));
        break;
    }
    }
    _xbee.send(req);
}

void CrcXbee::UpdateData(uint8_t batStatus)
{
    unsigned long now = millis();

    _xbee.readPacket();
    XBeeResponse resp = _xbee.getResponse();
    if (resp.isAvailable()) {
        if (resp.getApiId() == ZB_RX_RESPONSE) {
            ZBRxResponse rx = ZBRxResponse();
            resp.getZBRxResponse(rx);
            if (rx.getData()[0] & SPECIAL_OP) {
                const void* BOOTLOADER = (void*)0x1F000; // TODO: check fuses?
                switch (rx.getData()[0]) {
                case SPECIAL_OP_RESET:
                    if (rx.getDataLength() == 1)
                        goto* BOOTLOADER;
                default: /* Unknown special op */ break;
                }
            } else {
                _state.deserialize_update(rx.getData());
            }
            _lastUpdate         = millis();
            _communicationValid = true;
        } else if (resp.getApiId() == AtCommandResponse::API_ID) {
            AtCommandResponse rx;
            resp.getAtCommandResponse(rx);
            if (_addrInitState != READY)
                HandleInit(&rx);
        }
    } else if (resp.isError()) {
        Serial.print("XBee error: ");
        Serial.println(resp.getErrorCode());
    } else if ((unsigned long)(now - _lastUpdate) > _maxNoUpdateDelay) {
        ResetState();
        _communicationValid = false;
    }

    if (now - _lastBatStatus > _batStatusDelay && _addrInitState == READY) {
        ZBTxRequest req = ZBTxRequest(_addr, &batStatus, 1);
        _xbee.send(req);
        _lastBatStatus = now;
    }
}

RemoteState CrcXbee::State() { return _state; }

void CrcXbee::ResetState() { _state = { 0 }; }

bool CrcXbee::IsCommValid() const { return _communicationValid; }
}
