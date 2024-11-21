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

static esp_err_t dpp_enrollee_bootstrap(void)
{
    /* Currently only supported method is QR Code */
    return esp_supp_dpp_bootstrap_gen(/*channels=*/"6", DPP_BOOTSTRAP_QR_CODE, /*key=*/NULL, "WLED-Dial");
}

wifi_config_t s_dpp_wifi_config;

static void dpp_enrollee_event_cb(esp_supp_dpp_event_t event, void *data)
{
    static int s_retry_num;

    switch (event) {
    case ESP_SUPP_DPP_URI_READY:
        if (data != NULL) {
            ESP_LOGI(TAG, "Generated qr code data: %s", (char*)data);
            strncpy(qrCodeData, (const char*) data, 128);
        }
        break;
    case ESP_SUPP_DPP_CFG_RECVD:
        memcpy(&s_dpp_wifi_config, data, sizeof(s_dpp_wifi_config));
        esp_wifi_set_config(WIFI_IF_STA, &s_dpp_wifi_config);
        ESP_LOGI(TAG, "DPP Authentication successful, connecting to AP : %s",
                 s_dpp_wifi_config.sta.ssid);
        s_retry_num = 0;
        esp_wifi_connect();
        break;
    case ESP_SUPP_DPP_FAIL:
        if (s_retry_num < 5) {
            ESP_LOGI(TAG, "DPP Auth failed (Reason: %s), retry...", esp_err_to_name((int)data));
            ESP_ERROR_CHECK(esp_supp_dpp_start_listen());
            s_retry_num++;
        } else {
            //xEventGroupSetBits(s_dpp_event_group, DPP_AUTH_FAIL_BIT);
        }
        break;
    default:
        break;
    }
}

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
    ESP_ERROR_CHECK(esp_supp_dpp_init(dpp_enrollee_event_cb));
    ESP_ERROR_CHECK(dpp_enrollee_bootstrap());
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