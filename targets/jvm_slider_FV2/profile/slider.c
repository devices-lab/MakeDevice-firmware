#include "jdprofile.h"

FIRMWARE_IDENTIFIER(0x3799c72d, "JVM Slider");

void app_init_services(void) {
    potentiometer_init(NO_PIN, PA_2, PA_3);  // L, M, H
}
