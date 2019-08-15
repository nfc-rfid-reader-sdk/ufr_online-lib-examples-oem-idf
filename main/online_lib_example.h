#ifndef ONLINE_LIB_EXAMPLE_H
#define ONLINE_LIB_EXAMPLE_H
#include "esp32_digital_led_lib.h"

#define BUFFER_SIZE 2048

strand_t LED[] = {{.rmtChannel = 1, .gpioNum = 13, .ledType = LED_WS2812B_V3, .brightLimit = 1, .numPixels =  2, .pixels = NULL, ._stateVars = NULL}};

static void initialise_rgb();

static void set_led(uint8_t red1, uint8_t green1, uint8_t blue1, uint8_t red2, uint8_t green2, uint8_t blue2);

static void card_id_ex_task(void *pvParameters);

#endif
