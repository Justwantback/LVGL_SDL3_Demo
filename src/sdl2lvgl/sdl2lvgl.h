#ifndef __SDL2LVGL_H__
#define __SDL2LVGL_H__

#include "lvgl/lvgl.h"
#include "SDL3/SDL.h"

/* ================= 配置 ================= */
#define WINDOW_NAME "MQ80_UI_Demo LVGL9.2"
#define WIDTH   320
#define HEIGHT  240
#define BUF_HEIGHT 40
#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_ARGB8888))


int sdl_init(void);
void disp_flush(lv_display_t *disp,const lv_area_t *area,uint8_t *px_map);
void lv_port_disp_init(void);
void mouse_read_cb(lv_indev_t *indev,lv_indev_data_t *data);
void lv_port_input_init(void);
void sdl_handle_event(SDL_Event *e);
void sdl_handler_loop();

#endif



