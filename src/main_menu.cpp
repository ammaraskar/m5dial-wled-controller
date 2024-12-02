#include "main_menu.h"


lv_obj_t* main_menu(WledController& controller) {
    lv_obj_t* main_menu = lv_obj_create(NULL);

    lv_obj_t* on_off_button = lv_btn_create(main_menu);
    lv_obj_center(on_off_button);

    lv_obj_t* label = lv_label_create(on_off_button);
    lv_label_set_text(label, LV_SYMBOL_POWER);
    lv_obj_center(label);

    lv_obj_t* brightness_arc = lv_arc_create(main_menu);
    lv_obj_set_size(brightness_arc, 220, 220);
    lv_arc_set_rotation(brightness_arc, 135);
    lv_arc_set_bg_angles(brightness_arc, 0, 270);
    lv_arc_set_value(brightness_arc, 40);
    lv_obj_center(brightness_arc);

    return main_menu;
}
