#include "wifi.h"

#define SCAN_SIZE_LIST 20
wifi_scan_result wifi_scan_results[SCAN_SIZE_LIST];

wifi_scan_result* get_wifi_scan_results() {
    return wifi_scan_results;
}

#ifndef SIMULATOR

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

uint16_t perform_wifi_network_scan() {
    esp_wifi_scan_start(NULL, true);

    uint16_t ap_count = 0;

    uint16_t stored_ap_count = SCAN_SIZE_LIST;
    wifi_ap_record_t ap_info[SCAN_SIZE_LIST];
    memset(ap_info, 0, sizeof(ap_info));

    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&stored_ap_count, ap_info));
    ESP_LOGI(TAG, "Total APs scanned = %u, actual AP number ap_info holds = %u", ap_count, stored_ap_count);
    for (int i = 0; i < stored_ap_count; i++) {
        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
        ESP_LOGI(TAG, "Channel \t\t%d", ap_info[i].primary);

        memcpy(wifi_scan_results[i].ssid, ap_info[i].ssid, sizeof(wifi_scan_results[0].ssid));
    }

    return stored_ap_count;
}

void connect_to_wifi_network(wifi_scan_result* network, char* password) {
    wifi_config_t wifi_config = {};
    memcpy(wifi_config.sta.ssid, network->ssid, sizeof(wifi_config.sta.ssid));
    memcpy(wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());
}

#else

#include <SDL2/SDL.h>

void initialize_wifi_stack() {
    // no-op.
}

uint16_t perform_wifi_network_scan() {
    memcpy(wifi_scan_results[0].ssid, "Network foo", sizeof(wifi_scan_results[0].ssid));
    memcpy(wifi_scan_results[1].ssid, "The Pearl", sizeof(wifi_scan_results[0].ssid));
    memcpy(wifi_scan_results[2].ssid, "ATTfoobar-2Ghz", sizeof(wifi_scan_results[0].ssid));
    memcpy(wifi_scan_results[3].ssid, "A very very long long long WiFi name yup", sizeof(wifi_scan_results[0].ssid));
    memcpy(wifi_scan_results[4].ssid, "Bar", sizeof(wifi_scan_results[0].ssid));
    memcpy(wifi_scan_results[5].ssid, "Hello World", sizeof(wifi_scan_results[0].ssid));
    memcpy(wifi_scan_results[6].ssid, "Woo", sizeof(wifi_scan_results[0].ssid));

    // sleep for a bit to simulate scanning time.
    SDL_Delay(50);

    return 7;
}

void connect_to_wifi_network(wifi_scan_result* network, char* password) {
    // no-op for now.
}

#endif