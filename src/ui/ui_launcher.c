#include "ui_launcher.h"

/* =========================================================
 * UI
 * ========================================================= */
static void btn_event_cb(lv_event_t *e)
{
    lv_obj_t *label = lv_event_get_user_data(e);
    lv_label_set_text(label, "Button clicked!");
}

int ui_launcher_start()
{
    lv_obj_t *scr = lv_scr_act();

    lv_obj_t *btn = lv_button_create(scr);
    lv_obj_set_size(btn, 160, 60);
    lv_obj_center(btn);


    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Click me");
    lv_obj_center(label);

    lv_obj_add_event_cb(
        btn,
        btn_event_cb,
        LV_EVENT_CLICKED,
        label
    );
}