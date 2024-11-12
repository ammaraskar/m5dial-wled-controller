#include <M5Unified.hpp>
#include <freertos/FreeRTOS.h>
#include <lvgl.h>
#include <M5Dial-LVGL.h>
#include "setup_menu.h"

void main_task(void *)
{
    m5dial_lvgl_init();

    // TODO: only render the setup menu on first boot/uninitialized.
    lv_obj_t* setup = setup_menu();
    lv_screen_load(setup);

    for (;;)
    {
        m5dial_lvgl_next();
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