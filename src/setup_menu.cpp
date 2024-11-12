#include "setup_menu.h"
#include <stdint.h>


extern const uint8_t wifi_icon_start[] asm("_binary_resources_icons_wifi_png_start");

lv_obj_t* setup_menu() {
    lv_obj_t* setup_menu = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(setup_menu, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_text_color(setup_menu, lv_color_white(), LV_PART_MAIN);

    lv_obj_t* label = lv_label_create(setup_menu);
    lv_label_set_text_static(label, (LV_SYMBOL_WIFI " Setup"));
    lv_obj_remove_flag(label, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    // Make the label have a background and border.
    lv_color_t banner_background = lv_palette_main(LV_PALETTE_CYAN);
    lv_obj_set_style_pad_hor(label, 50, 0);
    lv_obj_set_style_pad_top(label, 10, 0);
    lv_obj_set_style_pad_bottom(label, 10, 0);
    lv_obj_set_style_bg_color(label, banner_background, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(label, LV_OPA_100, 0);

    return setup_menu;
}
