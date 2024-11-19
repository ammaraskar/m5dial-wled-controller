#include "setup_menu.h"
#include <stdint.h>
#include "wifi.h"

// Handles positioning elements in a circular way when scrolling.
static void scroll_event_cb(lv_event_t *e)
{
    lv_obj_t *cont = (lv_obj_t*) lv_event_get_target(e);
    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;
    lv_coord_t r = lv_obj_get_height(cont) * 5 / 10;

    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont);
    for (i = 0; i < child_cnt; i++) {
        lv_obj_t *child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        lv_coord_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /* Get the x of diff_y on a circle. */
        lv_coord_t x;
        /* If diff_y is out of the circle use the last point of the circle (the radius) */
        if (diff_y >= r) {
            x = r;
        } else {
            /* Use Pythagoras theorem to get x from radius and y */
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000);   /* Use lvgl's built in sqrt root function */
            x = r - res.i - 1; /* Added - 20 here to pull all a bit more to the left side */
        }

        /* Translate the item by the calculated X coordinate */
        lv_obj_set_style_translate_x(child, x, 0);
        lv_obj_set_style_translate_y(child, -13, 0);

        lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
    }
}

// An async method to actually perform the scan so we can update the GUI to show
// scanning while it happens.
static void scan_button_action_async(lv_timer_t*) {
    uint16_t num_networks = perform_wifi_network_scan();
    wifi_scan_result* networks = get_wifi_scan_results();

    lv_obj_t* wifi_list = lv_obj_create(lv_screen_active());
    lv_obj_set_style_border_side(wifi_list, LV_BORDER_SIDE_NONE, 0);
    lv_obj_set_style_bg_color(wifi_list, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(wifi_list, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_size(wifi_list, 245, 180);
    lv_obj_align(wifi_list, LV_ALIGN_TOP_MID, 0, 45);
    lv_obj_set_flex_flow(wifi_list, LV_FLEX_FLOW_COLUMN);
    lv_obj_add_event_cb(wifi_list, scroll_event_cb, LV_EVENT_SCROLL, NULL);
    lv_obj_set_scroll_dir(wifi_list, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(wifi_list, LV_SCROLL_SNAP_CENTER);

    for (int i = 0; i < num_networks; i++) {
        lv_obj_t* wifi_button = lv_button_create(wifi_list);
        lv_obj_set_width(wifi_button, lv_pct(100));

        lv_obj_t* label = lv_label_create(wifi_button);
        lv_label_set_text(label, (char*)networks[i].ssid);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
        lv_obj_set_width(label, lv_pct(100));
    }

    // Update button positions manually for the first draw.
    lv_obj_send_event(wifi_list, LV_EVENT_SCROLL, NULL);
    lv_obj_scroll_to_view(lv_obj_get_child(wifi_list, 0), LV_ANIM_OFF);
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
