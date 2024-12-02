#include "main_menu.h"

// TODO: move these globals to a class eventually.
lv_obj_t* on_off_button;
lv_obj_t* brightness_arc;
bool ledOn = false;


void set_controls_for_off_state() {
    lv_obj_set_style_bg_color(on_off_button, lv_palette_darken(LV_PALETTE_GREY, 1), 0);
    lv_obj_set_style_arc_color(brightness_arc, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_PART_INDICATOR);

    lv_obj_add_state(brightness_arc, LV_STATE_DISABLED);
}

void set_controls_for_on_state() {
    lv_obj_set_style_bg_color(on_off_button, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_arc_color(brightness_arc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);

    lv_obj_remove_state(brightness_arc, LV_STATE_DISABLED);
}

void update_with_lvgl_state(lv_timer_t* timer) {
    WledController* controller = (WledController*) timer->user_data;

    if (ledOn && !controller->is_wled_on()) {
        // Transition to off-state.
        set_controls_for_off_state();
        ledOn = false;
    } else if (!ledOn && controller->is_wled_on()) {
        // Transition to on-state.
        set_controls_for_on_state();
        ledOn = true;
    }

    // Set the arc to current brightness if we aren't focused on the brightness bar.
    if (lv_group_get_focused(lv_group_get_default()) != brightness_arc) {
        lv_arc_set_value(brightness_arc, controller->get_wled_brightness());
    }

    // Change brightness if it's different to what we're showing
    int32_t brightness = lv_arc_get_value(brightness_arc);
    if (brightness != controller->get_wled_brightness()) {
        controller->set_wled_brightness(brightness);
    }
}


// Make brightness arc have a little knob when focused.
static void brightness_arc_focused(lv_event_t* event) {
    lv_obj_t* brightness_arc = (lv_obj_t*) lv_event_get_target(event);

    if (lv_group_get_editing(lv_group_get_default())) {
        lv_obj_set_style_opa(brightness_arc, LV_OPA_100, LV_PART_KNOB);
    } else {
        lv_obj_set_style_opa(brightness_arc, LV_OPA_50, LV_PART_KNOB);
    }
}

// Remove brightness knob when it gets defocused.
static void brightness_arc_defocused(lv_event_t* event) {
    lv_obj_t* brightness_arc = (lv_obj_t*) lv_event_get_target(event);
    lv_obj_set_style_opa(brightness_arc, LV_OPA_0, LV_PART_KNOB);
}

static void on_off_button_clicked(lv_event_t* event) {
    WledController* controller = (WledController*) event->user_data;
    controller->turn_wled_on();
}


lv_obj_t* main_menu(WledController& controller) {
    lv_obj_t* main_menu = lv_obj_create(NULL);

    on_off_button = lv_btn_create(main_menu);
    lv_obj_center(on_off_button);
    lv_obj_add_event_cb(on_off_button, on_off_button_clicked, LV_EVENT_CLICKED, (void*) (&controller));

    lv_obj_t* label = lv_label_create(on_off_button);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_28, 0);
    lv_label_set_text(label, LV_SYMBOL_POWER);
    lv_obj_center(label);

    brightness_arc = lv_arc_create(main_menu);
    lv_obj_add_event_cb(brightness_arc, brightness_arc_focused, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(brightness_arc, brightness_arc_defocused, LV_EVENT_DEFOCUSED, NULL);
    lv_obj_send_event(brightness_arc, LV_EVENT_DEFOCUSED, NULL); // Fire a defocus event to start with.

    lv_arc_set_range(brightness_arc, 0, 255);  // WLED brightness ranges from 0 to 255.
    lv_obj_set_size(brightness_arc, 220, 220);
    lv_arc_set_rotation(brightness_arc, 135);
    lv_arc_set_bg_angles(brightness_arc, 0, 270);
    lv_arc_set_value(brightness_arc, 40);
    lv_obj_center(brightness_arc);

    // This fixes the brightness arc not being editable with encoder.
    lv_group_add_obj(lv_group_get_default(), brightness_arc);

    // Set a timer to sync the menu state with what is on screen.
    lv_timer_create(update_with_lvgl_state, 500, (void*) (&controller));
    set_controls_for_off_state();

    return main_menu;
}
