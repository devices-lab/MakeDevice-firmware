#include "jdprofile.h"

FIRMWARE_IDENTIFIER(0x3da07c70, "Dual Slider");

void app_init_services() {
    potentiometer_init(NO_PIN, PA_2, PA_3);
    potentiometer_init(NO_PIN, PA_4, PA_5);
}
