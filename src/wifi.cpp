#include "wifi.h"

char qrCodeData[128];
char* get_dpp_qrcode_data() {
    return qrCodeData;
}

#ifndef SIMULATOR

#include "esp_dpp.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include <cstring>

static const char *TAG = "WiFi";

void initialize_wifi_stack() {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_t* sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);
    ESP_ERROR_CHECK(esp_netif_set_hostname(sta_netif, "wled-dial"));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Check if there is an existing WiFi network stored.
    wifi_config_t wifi_config = {};
    esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
    if (wifi_config.sta.ssid[0] != 0) {
        ESP_LOGI(TAG, "Connecting to previously stored WiFi network %s", (char*)wifi_config.sta.ssid);
        esp_wifi_connect();
    }
}

#else

#include <SDL2/SDL.h>

void initialize_wifi_stack() {
    strcpy(qrCodeData, "dpp://xyz"); 
}

#endif