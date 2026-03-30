#define PIN_LED PB_8

#define PIN_SCL PA_11
#define PIN_SDA PA_12
#define I2C_AF LL_GPIO_AF_6
#define I2C_IDX 2

#define PIN_ACC_INT PC_15

#define UART_PIN PB_6
#define UART_PIN_AF LL_GPIO_AF_0
#define USART_IDX 1

// This clocksource isn't defined for our chip, so it results in a compiler error.
// We don't need this exact clocksource though
#define LL_RCC_I2C2_CLKSOURCE_HSI NULL
