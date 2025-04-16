#include "jdprofile.h"

FIRMWARE_IDENTIFIER(0x3a90f679, "JM dual button 0.1");

void app_init_services() {
    button_init(PA_4, 0, -1);
    button_init(PA_5, 0, -1);
}
