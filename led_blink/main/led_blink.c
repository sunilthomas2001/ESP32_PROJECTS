
#include <stdio.h>
#include <driver/gpio.h>// esp gpio header file
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LED_PIN GPIO_NUM_2

void app_main(void)

{

gpio_reset_pin(LED_PIN); // RESET THE PIN TO ELIMINATE GARBAGE

gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT); // SETTING GPIO PIN 2 AS OUTPUT

while(1)
{
gpio_set_level (LED_PIN, 1); //LED ON

vTaskDelay (pdMS_TO_TICKS(1000)); //DELAY OF 1000MS

gpio_set_level (LED_PIN, 0); //LED OFF

vTaskDelay (pdMS_TO_TICKS(1000));//DELAY OF 1000MS
}

}

    /*


    #include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void app_main(void)
{
    while(1)
    {
        printf("RUNNING...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
*/