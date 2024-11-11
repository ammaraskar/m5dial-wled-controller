#include "setup_menu.hpp"


extern const uint8_t wifi_icon_start[] asm("_binary_resources_icons_wifi_png_start");
extern const uint8_t wifi_icon_end[] asm("_binary_resources_icons_wifi_png_end");


void SetupMenu::initialDraw() {
    _gfx.startWrite();

    _gfx.setTextColor(TFT_WHITE);

    _gfx.drawString("WiFi", _gfx.width() / 2, 20);
    _gfx.drawString("Setup", _gfx.width() / 2, 55);

    ESP_LOGW("SetupMenu", "calling drawPng, len: %d", (wifi_icon_end - wifi_icon_start));
    bool drawRes = _gfx.drawPng(wifi_icon_start, (wifi_icon_end - wifi_icon_start), _gfx.width() / 2, 60);
    ESP_LOGW("SetupMenu", "drawPng result: %d", drawRes);

    _gfx.endWrite();
}
