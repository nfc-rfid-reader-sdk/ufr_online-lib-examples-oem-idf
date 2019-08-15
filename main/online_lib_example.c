#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_timer.h"
#include <stdlib.h>
#include <ctype.h>
#include "esp_event_loop.h"
#include "esp_log.h"
#include "uFCoder.h"
#include "online_lib_example.h"



void app_main()
{

	/*INITIALIZE RGB*/
	initialise_rgb();

     /* Set both RGB LEDs to white (75, 75, 75) */
    set_led(75,75,75,75,75,75);

    /*Open uFR Reader*/
    ReaderOpen();
   
	/* Call ReaderUISignal */
    ReaderUISignal(2, 2);

    /* Create card_id_ex_task */
	xTaskCreate(card_id_ex_task, "card_id_ex_task", BUFFER_SIZE, NULL, 5, NULL);


}

/* Card UID polling (200ms) */
static void card_id_ex_task(void *pvParameters)
{
    UFR_STATUS status;
    uint8_t uid[10];
    uint8_t temp_uid[10];
    uint8_t uid_size, card_type;  

    while (1)
    {
        status = GetCardIdEx(uid, &uid_size, &card_type);
        if(status == UFR_OK)
        {
            /* Check if last UID is different from current UID */
            if(memcmp(uid, temp_uid, uid_size) != 0)
            {
                /* If card is present wirte UID on LOG output (USB or UART_0) */
                ESP_LOG_BUFFER_HEX("UID", uid, uid_size);

                /* Beep on card present */
                ReaderUISignal(0, 1);

                /* Turn green LED on card present for 500ms */
                set_led(0,75,0,0,75,0);
                vTaskDelay(500 / portTICK_PERIOD_MS);
                set_led(75,75,75,75,75,75);
            }
             memcpy(temp_uid, uid, uid_size);
        }
        else
        {
            memset(uid, 0xff, 10);
            memset(temp_uid, 0x00, 10);
        }
               
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);    
}

/* Initialise RGB LED driver */
static void initialise_rgb()
{
	gpio_pad_select_gpio((gpio_num_t)13);
	gpio_set_direction((gpio_num_t)13, GPIO_MODE_OUTPUT);
	gpio_set_level((gpio_num_t)13, 0);
	digitalLeds_initStrands(LED, 1);
	
	LED[0].pixels[1] = pixelFromRGB(75, 75, 75);
	LED[0].pixels[0] = pixelFromRGB(75, 75, 75);
	digitalLeds_updatePixels(&LED[0]);
}

/* Set LED color */
static void set_led(uint8_t red1, uint8_t green1, uint8_t blue1, uint8_t red2, uint8_t green2, uint8_t blue2)
{
    LED[0].pixels[1] = pixelFromRGB(red1, green1, blue1);
	LED[0].pixels[0] = pixelFromRGB(red2, green2, blue2);
	digitalLeds_updatePixels(&LED[0]);
}


