#include <stdio.h>
#include <driver/gpio.h>// esp gpio header file
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define LED_PIN GPIO_NUM_4
#define PB_PIN GPIO_NUM_34
void app_main(void)
{
gpio_reset_pin(LED_PIN); // RESET THE PIN TO ELIMINATE GARBAGE
gpio_reset_pin(PB_PIN); // RESET THE PIN TO ELIMINATE GARBAGE
gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT); // SETTING GPIO PIN 4 AS OUTPUT
gpio_set_direction(PB_PIN, GPIO_MODE_INPUT); // SETTING GPIO PIN 34 AS INPUT
while(1)
{
    if(gpio_get_level(PB_PIN))
    {
        gpio_set_level (LED_PIN, 1); //LED ON
    }
    else
    {
        gpio_set_level (LED_PIN, 0); //LED OFF
    }
        vTaskDelay (pdMS_TO_TICKS(1000)); //DELAY OF 1000MS

}
}