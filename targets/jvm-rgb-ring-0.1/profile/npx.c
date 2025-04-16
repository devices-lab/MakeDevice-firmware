#include "jdprofile.h"
#include "jacdac/dist/c/led.h"

FIRMWARE_IDENTIFIER(0x378af8a6, "JVM RGB LED RING v2.1");

void app_init_services() {
    leddisplay_init(LIGHT_TYPE_WS2812B_GRB, 8, 200, JD_LED_VARIANT_RING);
}
