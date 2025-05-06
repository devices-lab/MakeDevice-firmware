#include "jdprofile.h"

FIRMWARE_IDENTIFIER(0x33dbec8c, "JVM Developer RGB 117 v1.0");

static const led_params_t params = {
    .pin_r = PA_6,
    .pin_g = PA_8,
    .pin_b = PA_7,
    .active_high = 0, // set to 0 for sinking current
    .led_count = 1, // how many LEDs there are
    .max_power = 20, // in mA
    .mult_r = 250, // brightness calibration
    .mult_g = 250,
    .mult_b = 250 
    
};

void app_init_services() {

    led_service_init(&params);
}
