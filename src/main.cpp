#include <M5Unified.hpp>
#include <lvgl.h>
#include <M5Dial-LVGL.h>
#include "setup_menu.h"

void initial_setup() {
    m5dial_lvgl_init();

    // TODO: only render the setup menu on first boot/uninitialized.
    lv_obj_t* setup = setup_menu();
    lv_screen_load(setup);
}

void loop() {
    m5dial_lvgl_next();
}

// Define different ways to call the above functions depending on if we're
// running on the board or in the simulator.
#if defined(ESP_PLATFORM)
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