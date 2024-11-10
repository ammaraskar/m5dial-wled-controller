#include "M5Dial.h"
#include "setup_menu/setup_menu.hpp"

M5Canvas canvas;

void setup() {
    auto cfg = M5.config();
    M5Dial.begin(cfg, /*enableEncoder=*/true, /*enableRFID=*/false);
    M5Dial.Display.setTextColor(GREEN);
    M5Dial.Display.setTextDatum(middle_center);
    M5Dial.Display.setFont(&fonts::efontCN_24);
    M5Dial.Display.setTextSize(2);

    M5Dial.Speaker.setAllChannelVolume(255);

    M5Dial.Display.fillScreen(TFT_BLACK);
}

bool setupDone = false;

void loop() {
    M5Dial.update();

    // TODO: Go into first time setup if we don't have a LED configured.
    if (!setupDone) {
        SetupMenu setupMenu(M5Dial.Display);
        setupDone = true;
    }
}