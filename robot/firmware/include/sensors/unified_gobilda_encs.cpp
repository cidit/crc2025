
#include <sensors/sensor.hpp>
#include <sensors/gobuilda_rotary_enc_data.hpp>
#include <communication/enco_peripherals.hpp>
#include <util/lifecycle.hpp>
#include <util/misc.hpp>
#include <util/timer.hpp>

struct enco_pins {
    pin_t a, b;
};


// TODO: not a good direction to take the code. 

/**
 * Unified Gobilda Encoders
 */
class UnifiedGbdEncos : public Lifecycle {

    // TODO: prob should find a way to init properly?
    GobuildaRotaryEncoderData _encoder_data[ENCO_NUM];

    bool _reverse_mask[ENCO_NUM];

    Timer _poll_timer;

    UnifiedGbdEncos(
        enco_pins enco_pins[ENCO_NUM], 
        const Timer& poll_timer,
        bool reverse_mask[ENCO_NUM] = {0}
    ): _poll_timer(poll_timer) {
        set_reverse_mask(reverse_mask);
    }

    void set_reverse_mask(bool reverse_mask[ENCO_NUM]) {
        for (auto i = 0; i < ENCO_NUM; i++) {
            _reverse_mask[i] = reverse_mask[i];
        }
    }

    void begin() override {
        // same as in main-test-spi_crcduino.cpp
        master_enco_spi_init();
        SPI.begin();
    }

    void update() override {
        if (!_poll_timer.is_time()) {
            return;
        }
        int32_t df[ENCO_NUM];
        // TODO: for some reason, the values seem to be multiplied by 256
        retrieve_df(df);
        
    }

};