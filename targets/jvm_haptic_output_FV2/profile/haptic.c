#include "jdprofile.h"
#include "interfaces/jd_pwm.h"
#include "jdstm.h"

FIRMWARE_IDENTIFIER(0x3f7cbf96, "Haptic");

// Written for the lcm1027a2445f, but generic enough for other haptic motors.

#define MAX_DURATION 200
#define HAPTIC_PERIOD 5000

static uint8_t pwm_id;
static bool inited = false;
static void haptic_init(void) {
    pin_setup_output(PIN_P0);
    pwm_id = jd_pwm_init(PIN_P0, HAPTIC_PERIOD, 0, cpu_mhz);
    inited = true;
}

static bool writing_amplitude = false; // Not sure this is necessary to have, but just in case.
static int haptic_write_amplitude(uint8_t amplitude, uint32_t duration_ms) {
    if (writing_amplitude)
        return 0;
    writing_amplitude = true;

    if (!inited)
        haptic_init();

    if (amplitude == 0 || duration_ms == 0)
        return 0;

    if (duration_ms > MAX_DURATION)
        return -2;

    uint32_t duty = (HAPTIC_PERIOD * amplitude) >> 8;
    jd_pwm_set_duty(pwm_id, duty);

    jd_pwm_enable(pwm_id, 1);
    target_wait_us(duration_ms * 1000);
    jd_pwm_enable(pwm_id, 0);

    writing_amplitude = false;
    return 0;
}

const vibration_motor_api_t LCM1027A2445F = {
    .init = haptic_init,
    .write_amplitude = haptic_write_amplitude,
    .max_duration = MAX_DURATION,
};

void app_init_services(void) {
    vibration_motor_init(&LCM1027A2445F);
}
