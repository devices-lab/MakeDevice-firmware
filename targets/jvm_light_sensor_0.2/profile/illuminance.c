#include "jdprofile.h"
#include "math.h"
#include "interfaces/jd_adc.h"

FIRMWARE_IDENTIFIER(0x3872b0c7, "Light Sensor");

/**
 * This is a driver for a phototransistor jacdac virtual module.
 * It was designed for the SMD1206-30 (https://www.lcsc.com/datasheet/C250857.pdf)
 * Where the emitter is tied to 3v3, the collector is tied to vOUT (where we read from) and GND (via
 * 10k resistor). This layout is shown in page 4 of the above datasheet. The exception is that we
 * have Vdd@3v3 instead of 5v This supresses the dynamic range of the phototransistor, resulting in
 * a significant output error at higher light intensities.
 *
 * This driver should work for other phototransistors, including at different collector vDD.
 * You should change smd1206_30_voltage_to_lux if you use a different phototransistor.
 * The sample time may also be worth changing.
 */

typedef struct state {
    uint8_t inited;
    env_reading_t lightLevelLX;
    uint32_t samplePeriodUS;
    uint32_t nextSampleTimeUS;
} ctx_t;

inline static float smd1206_30_voltage_to_lux(uint16_t adc_volt);
static void phototransistor_init(void);
static void phototransistor_process(void);
static bool phototransistor_is_present(void);
static void *phototransistor_lightLevelLX(void);

/**
 * y_{1}\sim ax_{1}^{b}
 * Regression calculated by comparing adc against lux meter (LX1330B)
 * Unfortunately the collector for this SMD1206 is tied to 3v3 instead of 5v
 * SMD1206 is https://www.lcsc.com/datasheet/C250857.pdf
 */
inline static float smd1206_30_voltage_to_lux(uint16_t adc_volt) {
    return powf((24.81925 * adc_volt), 1.05638);
}

static ctx_t state = {0, {0, 0, 0, 0}, 0, 0};
static void phototransistor_init(void) {
    ctx_t *ctx = &state;

    if (!ctx->inited) {
        pin_setup_analog_input(LIGHT_LEVEL);
        ctx->lightLevelLX.min_value = 0;
        // max adc reading was 60, smd1206_30_voltage_to_lux(60) = 2248:
        ctx->lightLevelLX.max_value = 2248;
        ctx->samplePeriodUS = 9000;
        ctx->nextSampleTimeUS = now;

        ctx->inited = 1;
    }
}

/**
 * Takes a new sample every state.sampleTimeUS
 * Processes scales adc reading using smd1206_30_voltage_to_lux
 */
static void phototransistor_process(void) {
    phototransistor_init();

    if (jd_should_sample(&state.nextSampleTimeUS, state.samplePeriodUS)) {
        uint16_t v = adc_read_pin(LIGHT_LEVEL);
        ctx_t *ctx = &state;
        ctx->lightLevelLX.value = smd1206_30_voltage_to_lux(v);

        // TODO:
        //  Add a more accurate .error
        //  The error varies significantly with the reading
        //  On the SMD1206-30 compared to the LX1330B
        //  At ambient (LX1330B = 270lx) the SMD1206-30 error is as low as ~10%
        //  Beyond this it increases significantly.
        //  One reason for this is that the dynamic range of the SMD1206-30 has been significantly
        //  suppressed because it is tied to 3v3 instead of 5v
    }
}

static void *phototransistor_lightLevelLX(void) {
    ctx_t *ctx = &state;
    if (!ctx->inited)
        phototransistor_process();
    return &ctx->lightLevelLX;
}

static bool phototransistor_is_present(void) {
    return true;
}

const env_sensor_api_t phototransistor_api = {
    .init = phototransistor_init,
    .process = phototransistor_process,
    .get_reading = phototransistor_lightLevelLX,
    .is_present = phototransistor_is_present,
};

void app_init_services(void) {
    illuminance_init(&phototransistor_api);
}
