#include "jdprofile.h"
FIRMWARE_IDENTIFIER(0x347f56eb, "JVM Rotary + Btn v0.1");

void app_init_services() {
    rotaryencoder_init(PA_7, PA_6, 15, 0);
    button_init(PA_0, 0, -1);
}
