#include "jdprofile.h"
#include "jacdac/dist/c/relay.h"

FIRMWARE_IDENTIFIER(0x33837032, "Relay");

#define RELAY_LED PA_4
#define RELAY_DRIVE PA_4
#define RELAY_FEEDBACK -1

// solid state relay
static const relay_params_t params = {
    .relay_variant = JD_RELAY_VARIANT_ELECTROMECHANICAL,
    .max_switching_current = 2000,
    .pin_relay_drive = RELAY_DRIVE,
    .pin_relay_feedback = RELAY_FEEDBACK,
    .pin_relay_led = RELAY_LED,
    .drive_active_lo = 0,
    .led_active_lo = 0,
    .initial_state = false
};

void app_init_services() {
    relay_service_init(&params);
}
