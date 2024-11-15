#include "setup_menu.h"
#include <stdint.h>
#include "wifi.h"

// An async method to actually perform the scan so we can update the GUI to show
// scanning while it happens.
static void scan_button_action_async(lv_timer_t*) {
    uint16_t num_networks = perform_wifi_network_scan();
    wifi_scan_result* networks = get_wifi_scan_results();

    lv_obj_t* wifi_list = lv_list_create(lv_screen_active());
    lv_obj_set_size(wifi_list, 210, 130);
    lv_obj_align(wifi_list, LV_ALIGN_TOP_MID, 0, 60);

    lv_list_add_text(wifi_list, "Networks");

    for (int i = 0; i < num_networks; i++) {
        lv_obj_t* wifi_button = lv_list_add_button(wifi_list, NULL, (char*)networks[i].ssid);
    }
}

static void scan_button_cb(lv_event_t* e) {
    lv_obj_t* btn = (lv_obj_t*) lv_event_get_target(e);
    lv_obj_t* label = lv_obj_get_child(btn, 0);
    lv_label_set_text(label, "Scanning...");

    // Run the actual scan a little bit later.
    lv_timer_t* timer = lv_timer_create(scan_button_action_async, 100, NULL);
    lv_timer_set_repeat_count(timer, 1);
}

lv_obj_t* setup_menu() {
    lv_obj_t* setup_menu = lv_obj_create(NULL);

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

    // Add a centered scan button.
    lv_obj_t* scan_button = lv_btn_create(setup_menu);
    lv_obj_set_size(scan_button, 100, 50);
    lv_obj_align(scan_button, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(scan_button, scan_button_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* scan_button_label = label = lv_label_create(scan_button);
    lv_label_set_text(label, "Scan");
    lv_obj_center(scan_button_label);

    return setup_menu;
}
