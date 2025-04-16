#include "jdprofile.h"

FIRMWARE_IDENTIFIER(0x33af303b, "JVM single button 0.1");

void app_init_services() {
    button_init(PA_4, 0, -1);
}
