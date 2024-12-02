#ifndef __WLED_H__
#define __WLED_H__

#include "ArduinoJson.hpp"
using ArduinoJson::JsonDocument;
using ArduinoJson::JsonObject;

/// @brief Interfaces with the WLED websocket API to retrieve info and update WLED.
class WledController {
public:
    virtual void turn_wled_on() = 0;
    virtual void set_wled_brightness(int16_t) = 0;

    virtual bool is_wled_on() = 0;
    virtual int16_t get_wled_brightness() = 0;
};



#ifndef SIMULATOR

#include "esp_websocket_client.h"

class EspWledController : public WledController {
public:
    explicit EspWledController();

    void websocket_event_handler(esp_event_base_t base, int32_t event_id, void *event_data);
    void on_wled_json_data(JsonDocument& doc);


    void turn_wled_on() override;
    void set_wled_brightness(int16_t) override;

    bool is_wled_on() override { return _is_wled_on; }
    int16_t get_wled_brightness() override { return _brightness; }

private:
    bool _is_wled_on;
    int16_t _brightness;

    esp_websocket_client_handle_t _client;
};

#else

class MockWledController : public WledController {
public:
    explicit MockWledController() {}

    void turn_wled_on() override { _is_on = true; }
    void set_wled_brightness(int16_t brightness) override { _brightness = brightness; }

    bool is_wled_on() override { return _is_on; }
    int16_t get_wled_brightness() override { return 200; }

private:
    bool _is_on = false;
    int16_t _brightness = 200;
};

#endif

#endif  // __WLED_H__
