#include <stdbool.h>
#include "SDL3/SDL.h"
#include "lvgl/lvgl.h"
#include "sdl2lvgl/sdl2lvgl.h"
#include "ui/ui_launcher.h"








/* =========================================================
 * 主函数
 * ========================================================= */
int main(int argc, char *argv[])
{
    if (sdl_init() != 0) return -1;

    lv_init();
    lv_port_disp_init();
    lv_port_input_init();
    ui_launcher_start();
    sdl_handler_loop();
    return 0;
}
