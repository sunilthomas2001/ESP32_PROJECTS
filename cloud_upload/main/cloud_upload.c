#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_http_client.h"

#define SSID "AndroidAP_5878"
#define PASSWORD "st12345678"

// function to send data to cloud
void send_to_cloud(int temp)
{
char url[200];
sprintf(url,
"http://api.thingspeak.com/update?api_key=8MM3ZQSAY8IH0GEU&field1=%d",
temp);
esp_http_client_config_t cloud_config = {
.url = url,
};
esp_http_client_handle_t client = esp_http_client_init(&cloud_config);
esp_http_client_perform(client);
esp_http_client_cleanup(client);
}



// wifi event handler
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
// wifi configuration
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


//adc configuration

adc_oneshot_unit_handle_t handle; // created a handle
adc_oneshot_unit_init_cfg_t init_config; //declaration of init_config

init_config.unit_id = ADC_UNIT_1; // selected ADC 1 Unit as out adcconfig.atten = ADC_ATTEN_DB_12;
adc_oneshot_new_unit(&init_config, &handle); //passing handle config to new unit

adc_oneshot_chan_cfg_t adc_config;
adc_config.atten = ADC_ATTEN_DB_12; // Voltage range 0-3.3 v can be counted as 0-4095;

adc_config.bitwidth = ADC_BITWIDTH_DEFAULT; //(12BIT ADC)
adc_oneshot_config_channel (handle, ADC_CHANNEL_6, &adc_config); // passing handle, channel value and configuration settings to channel configuration structure.


while(1)
{
int raw = 0;
adc_oneshot_read(handle, ADC_CHANNEL_6, &raw);
printf("Raw ADC Value = %d\n", raw);
send_to_cloud(raw);
vTaskDelay (pdMS_TO_TICKS(15000));}
}