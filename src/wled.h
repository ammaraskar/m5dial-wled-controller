#ifndef __WLED_H__
#define __WLED_H__

#include "ArduinoJson.hpp"
using ArduinoJson::JsonDocument;
using ArduinoJson::JsonObject;

/// @brief Interfaces with the WLED websocket API to retrieve info and update WLED.
class WledController {
public:
};



#ifndef SIMULATOR

#include "esp_websocket_client.h"

class EspWledController : public WledController {
public:
    explicit EspWledController();

    void websocket_event_handler(esp_event_base_t base, int32_t event_id, void *event_data);
    void on_wled_json_data(JsonDocument& doc);

private:
    bool _is_wled_on;
    int16_t _brightness;

    esp_websocket_client_handle_t _client;
};

#else

class MockWledController : public WledController {
public:
    explicit MockWledController() {}
};

#endif

#endif  // __WLED_H__
