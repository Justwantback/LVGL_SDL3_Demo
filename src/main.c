#include <stdbool.h>
#include "SDL3/SDL.h"
#include "../third_party/lvgl/lvgl.h"
#include "sdl2lvgl/sdl2lvgl.h"





/* =========================================================
 * UI
 * ========================================================= */
static void btn_event_cb(lv_event_t *e)
{
    lv_obj_t *label = lv_event_get_user_data(e);
    lv_label_set_text(label, "Button clicked!");
}

static void app_ui(void)
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



/* =========================================================
 * 主函数
 * ========================================================= */
int main(int argc, char *argv[])
{
    if (sdl_init() != 0) return -1;

    lv_init();
    lv_port_disp_init();
    lv_port_input_init();
    app_ui();
    sdl_handler_loop();
    return 0;
}
