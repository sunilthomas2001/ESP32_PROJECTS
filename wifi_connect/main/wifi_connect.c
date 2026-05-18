#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"

#define SSID "AndroidAP_5878"
#define PASSWORD "st12345678"

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id,void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
{
printf("Wifi started \n");
esp_wifi_connect();
}
else if (event_base==IP_EVENT && event_id==IP_EVENT_STA_GOT_IP)
{
printf("Connection successful\n");
ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
printf("Got IP:" IPSTR "\n", IP2STR(&event->ip_info.ip));
} 
else if(event_base ==WIFI_EVENT && event_id==WIFI_EVENT_STA_DISCONNECTED)
{
printf("wifi disconnected \n");
wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*) event_data;
printf("Reason:%d \n", event->reason);
}
}

void app_main(void)
{
nvs_flash_init();
esp_netif_init();

esp_event_loop_create_default();
esp_netif_create_default_wifi_sta();

wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
esp_wifi_init(&cfg);

esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

wifi_config_t config = {
.sta = {
.ssid = SSID,
.password = PASSWORD,
},
};
esp_wifi_set_mode(WIFI_MODE_STA);
esp_wifi_set_config(WIFI_IF_STA, &config);
esp_wifi_start();

while(1)
{
vTaskDelay (pdMS_TO_TICKS(1000));
}
}