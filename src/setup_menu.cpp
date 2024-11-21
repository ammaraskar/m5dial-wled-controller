#include "setup_menu.h"
#include <stdint.h>
#include "wifi.h"

// Create and return the setup_menu.
lv_obj_t* setup_menu() {
    lv_obj_t* setup_menu = lv_obj_create(NULL);

    // Disable left-right scrolling on the setup menu.
    lv_obj_set_scroll_dir(setup_menu, LV_DIR_VER);

    lv_obj_set_style_bg_color(setup_menu, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_text_color(setup_menu, lv_color_white(), LV_PART_MAIN);

    lv_obj_t* label = lv_label_create(setup_menu);
    lv_label_set_text_static(label, (LV_SYMBOL_WIFI " Setup"));
    lv_obj_remove_flag(label, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    // Make the label have a background and border.
    lv_color_t banner_background = lv_palette_main(LV_PALETTE_BLUE_GREY);
    lv_obj_set_style_pad_hor(label, 50, 0);
    lv_obj_set_style_pad_top(label, 10, 0);
    lv_obj_set_style_pad_bottom(label, 10, 0);
    lv_obj_set_style_bg_color(label, banner_background, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(label, LV_OPA_100, 0);
    lv_obj_set_style_radius(label, 200, LV_PART_MAIN);

    // Add a centered qr code.
    lv_obj_t* qr = lv_qrcode_create(setup_menu);
    lv_qrcode_set_size(qr, 165);
    lv_qrcode_set_dark_color(qr, lv_color_white());
    lv_qrcode_set_light_color(qr, lv_color_black());

    //const char* data = "https://lvgl.io";
    char* data = get_dpp_qrcode_data();
    lv_qrcode_update(qr, data, strlen(data));
    lv_obj_align(qr, LV_ALIGN_CENTER, 0, 5);

    return setup_menu;
}
