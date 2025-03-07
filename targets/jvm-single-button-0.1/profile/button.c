#include "jdprofile.h"

FIRMWARE_IDENTIFIER(0x30aad584, "JM dual button 0.1");

void app_init_services() {
    button_init(PA_4, 0, -1);
}
