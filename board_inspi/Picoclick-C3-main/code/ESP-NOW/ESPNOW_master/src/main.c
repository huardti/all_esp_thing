/* Basic ESPNOW Example - Master

   This is a very basic example of receiving data from the complementary Slave example.
   By default, it receives an unencrypted data packet containing a random value and the state of a button from the slave, via the broadcast mac address.

   To customise for your data:
    - Alter my_data_t in espnow_basic_config
    - Alter my_data_receive my_master.c
    - Alter my_data_populate my_slave.c

   For basic demonstration, do not edit this file
*/

#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_now.h"
#include <driver/gpio.h>

#include "sdkconfig.h"
#include "espnow_basic_config.h"
#include "led_strip.h"

static const char *TAG = "ESPNOW_Master";

#define LED_PIN 22

static led_strip_t *pStrip_a;
static uint8_t s_led_state = 0;
#define LED_PIN_WS2812  14
#define NUM_LEDS        16
#define LED_RMT_CHANNEL 0

static unsigned long t_recv = 0;
static bool recv_msg = false;
static bool led_state = false;
static uint8_t led_counter = 0;

void my_data_receive(const uint8_t *sender_mac_addr, const my_data_t *data);

static xQueueHandle s_recv_queue;

typedef struct {
    uint8_t sender_mac_addr[ESP_NOW_ETH_ALEN];
    my_data_t data;
} recv_packet_t;

static void queue_process_task(void *p){
    static recv_packet_t recv_packet;
    ESP_LOGI(TAG, "Listening");
    for(;;){
        if(xQueueReceive(s_recv_queue, &recv_packet, portMAX_DELAY) != pdTRUE){
            continue;
        }
        // Refer to user function
        my_data_receive(recv_packet.sender_mac_addr, &recv_packet.data);
    }
}

#define MY_ESPNOW_WIFI_MODE WIFI_MODE_STA
#define MY_ESPNOW_WIFI_IF   ESP_IF_WIFI_STA
// #define MY_ESPNOW_WIFI_MODE WIFI_MODE_AP
// #define MY_ESPNOW_WIFI_IF   ESP_IF_WIFI_AP

static void recv_cb(const uint8_t *mac_addr, const uint8_t *data, int len){
    static recv_packet_t recv_packet;

    led_state = !led_state;
    gpio_set_level(LED_PIN, led_state);

    if(led_counter == 0){
        for(int i=0; i<NUM_LEDS; i++){
            pStrip_a->set_pixel(pStrip_a, i, 100, 0, 0);
            pStrip_a->refresh(pStrip_a, 50);
            vTaskDelay(20/portTICK_PERIOD_MS);
        }
    }
    else if(led_counter == 1){
        for(int i=0; i<NUM_LEDS; i++){
            pStrip_a->set_pixel(pStrip_a, i, 0, 100, 0);
            pStrip_a->refresh(pStrip_a, 50);
            vTaskDelay(20/portTICK_PERIOD_MS);
        }
    }
    else if(led_counter == 2){
        for(int i=0; i<NUM_LEDS; i++){
            pStrip_a->set_pixel(pStrip_a, i, 0, 0, 100);
            pStrip_a->refresh(pStrip_a, 50);
            vTaskDelay(20/portTICK_PERIOD_MS);
        }
    }

    led_counter++;
    if(led_counter == 2) led_counter = 0;

    ESP_LOGI(TAG, "%d bytes incoming from " MACSTR, len, MAC2STR(mac_addr));
    
    if(len != sizeof(my_data_t)){
        ESP_LOGE(TAG, "Unexpected data length: %d != %u", len, sizeof(my_data_t));
        return;
    }

    memcpy(&recv_packet.sender_mac_addr, mac_addr, sizeof(recv_packet.sender_mac_addr));
    memcpy(&recv_packet.data, data, len);
    if (xQueueSend(s_recv_queue, &recv_packet, 0) != pdTRUE) {
        ESP_LOGW(TAG, "Queue full, discarded");
        return;
    }
}

static void init_espnow_master(void){
    const wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
    ESP_ERROR_CHECK( esp_netif_init());
    ESP_ERROR_CHECK( esp_event_loop_create_default() );
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(MY_ESPNOW_WIFI_MODE) );
    ESP_ERROR_CHECK( esp_wifi_start() );
#if MY_ESPNOW_ENABLE_LONG_RANGE
    ESP_ERROR_CHECK( esp_wifi_set_protocol(MY_ESPNOW_WIFI_IF, WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N|WIFI_PROTOCOL_LR) );
#endif
    ESP_ERROR_CHECK( esp_now_init() );
    ESP_ERROR_CHECK( esp_now_register_recv_cb(recv_cb) );
    ESP_ERROR_CHECK( esp_now_set_pmk((const uint8_t *)MY_ESPNOW_PMK) );
}


static void configure_led(void){
    pStrip_a = led_strip_init(LED_RMT_CHANNEL, LED_PIN_WS2812, NUM_LEDS);
    pStrip_a->clear(pStrip_a, 50);
}

void app_main(void){
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, true);

    s_recv_queue = xQueueCreate(10, sizeof(recv_packet_t));
    assert(s_recv_queue);
    BaseType_t err = xTaskCreate(queue_process_task, "recv_task", 8192, NULL, 4, NULL);
    assert(err == pdPASS);
    
    init_espnow_master();
    ESP_LOGI(TAG, "ESP Now Initialization done.");
    
    configure_led();

    for(int i=0; i<NUM_LEDS; i++){
        pStrip_a->set_pixel(pStrip_a, i, 0, 0, 50);
        pStrip_a->refresh(pStrip_a, 50);
        vTaskDelay(20/portTICK_PERIOD_MS);
    }
}