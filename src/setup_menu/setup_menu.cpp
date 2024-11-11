#include "setup_menu.hpp"


extern const uint8_t wifi_icon_start[] asm("_binary_resources_icons_wifi_png_start");


void SetupMenu::initialDraw() {
    _gfx.startWrite();

    // Background for top banner.
    _gfx.setColor(TFT_BROWN);
    _gfx.writeFillRect(0, 0, _gfx.width(), 40);

    _gfx.setTextColor(TFT_WHITE);
    _gfx.setColor(TFT_WHITE);

    // Wifi icon.
    bool drawRes = _gfx.drawPng(/*data=*/wifi_icon_start, /*len=*/~0u,
        /*x=*/(_gfx.width() / 2) - 40,
        /*y=*/10);

    // Setup text beneath icon.
    _gfx.setTextDatum(top_left);
    _gfx.drawString("SETUP", (_gfx.width() / 2) - 9, 11);

    _gfx.endWrite();
}
