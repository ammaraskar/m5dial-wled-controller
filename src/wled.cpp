#include "wled.h"

static const char *TAG = "WS";

#ifndef SIMULATOR

#include "esp_log.h"

#define WLED_SERVER "192.168.1.137"

// Just a plain C function that then invokes the C++ websocket event handler method.
static void websocket_event_handler_invoker(void* handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    EspWledController* controller = (EspWledController*) handler_args;
    controller->websocket_event_handler(base, event_id, event_data);
}

EspWledController::EspWledController() {
    esp_websocket_client_config_t ws_cfg = {};
    ws_cfg.uri = "ws://" WLED_SERVER "/ws";
    // Timeout connections and reconnect every 10s.
    ws_cfg.network_timeout_ms = 10000;
    ws_cfg.reconnect_timeout_ms = 10000;

    ESP_LOGI(TAG, "Connecting to websocket %s", ws_cfg.uri);
    _client = esp_websocket_client_init(&ws_cfg);

    ESP_ERROR_CHECK(
        esp_websocket_register_events(_client, WEBSOCKET_EVENT_ANY,
            websocket_event_handler_invoker, (void*) this));

    ESP_ERROR_CHECK(esp_websocket_client_start(_client));
}

void EspWledController::websocket_event_handler(esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    switch (event_id) {
    case WEBSOCKET_EVENT_BEGIN:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_BEGIN");
        break;
    case WEBSOCKET_EVENT_CONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_CONNECTED");
        break;
    case WEBSOCKET_EVENT_DATA:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DATA");
        ESP_LOGI(TAG, "Received opcode=%d", data->op_code);

        // Opcode 1 is a text frame and contains updates from wled.
        if (data->op_code == 0x1) {
            ESP_LOGW(TAG, "Received=%.*s\n\n", data->data_len, (char *)data->data_ptr);

            JsonDocument doc;
            deserializeJson(doc, (char *)data->data_ptr, data->data_len);
            on_wled_json_data(doc);
        }
    }
}

void EspWledController::on_wled_json_data(JsonDocument& doc) {
    // Update state variables if present.
    if (doc["state"].is<JsonObject>()) {
        _is_wled_on = doc["state"]["on"].as<bool>();
        _brightness = doc["state"]["bri"].as<signed int>();
    }

    ESP_LOGI(TAG, "Is WLED on? %s", doc["state"]["on"] ? "on" : "off");
}

void EspWledController::turn_wled_on() {
    const static char* payload = "{\"on\": true}";
    esp_websocket_client_send_text(_client, payload, strlen(payload), 1000);
    ESP_LOGI(TAG, "Sending turn on payload %s", payload);
}

void EspWledController::set_wled_brightness(int16_t brightness) {
    JsonDocument doc;
    doc["bri"] = brightness;

    std::string payload;
    serializeJson(doc, payload);

    esp_websocket_client_send_text(_client, payload.c_str(), payload.length(), 1000);
    ESP_LOGI(TAG, "Sending brightness payload %s", payload.c_str());
}

#else


#endif
