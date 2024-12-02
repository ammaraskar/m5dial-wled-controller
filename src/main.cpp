// Fixes undefined reference to WinMain issue
#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <M5Unified.hpp>
#include <lvgl.h>
#include <M5Dial-LVGL.h>
#include "setup_menu.h"
#include "main_menu.h"
#include "wifi.h"
#include "wled.h"


void initial_setup() {
    m5dial_lvgl_init();
    initialize_wifi_stack();  // Also initializes esp event loop.

    // Register an input group so interactive stuff can be controlled by the encoder.
    lv_group_t* group = lv_group_create();
    lv_group_set_default(group);
    for (lv_indev_t *indev = lv_indev_get_next(nullptr); indev != nullptr; indev = lv_indev_get_next(indev))
    {
        if (lv_indev_get_type(indev) == LV_INDEV_TYPE_ENCODER)
        {
            lv_indev_set_group(indev, group);
            break;
        }
    }

    #ifndef SIMULATOR
    EspWledController controller;
    #else
    MockWledController controller;
    #endif

    // TODO: only render the setup menu on first boot/uninitialized.
    bool needs_setup = false;
    if (needs_setup) {
        lv_obj_t* setup = setup_menu();
        lv_screen_load(setup);
    } else {
        lv_obj_t* main = main_menu(controller);
        lv_screen_load(main);
    }
}

void loop() {
    m5dial_lvgl_next();
}

// Define different ways to call the above functions depending on if we're
// running on the board or in the simulator.
#ifndef SIMULATOR
// Actual board.
#include <freertos/FreeRTOS.h>

void main_task(void *)
{
    initial_setup();
    for (;;)
    {
        loop();
    }
    vTaskDelete(nullptr);
}

extern "C"
{
    void app_main()
    {
        // Pin the GUI task to core 1, as most of the ESP S3 libraries do their
        // magic on core 0.
        xTaskCreatePinnedToCore(main_task, "main_task", 8192, nullptr, 1, nullptr, 1);
    }
}
#else
// Native/Simulator
int main_task(bool *running)
{
    initial_setup();
    for (; *running;)
    {
        loop();
    }
    return 0;
}

extern "C" int main(int, char**)
{
    return lgfx::Panel_sdl::main(main_task, 128);
}
#endif