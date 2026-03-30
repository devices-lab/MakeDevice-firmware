#include "jdprofile.h"
#include "jacdac/dist/c/ledstrip.h"

FIRMWARE_IDENTIFIER(0x3202e0cc, "RGB Strip");

void app_init_services() {
    ledstrip_init(JD_LED_STRIP_LIGHT_TYPE_WS2812B_GRB, 32, 200, JD_LED_STRIP_VARIANT_STRIP);
}
