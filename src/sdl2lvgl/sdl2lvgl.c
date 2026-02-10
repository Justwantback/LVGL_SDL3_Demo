#include "sdl2lvgl.h"
#include "SDL3/SDL.h"
#include "lvgl/lvgl.h"

/* ================= SDL 全局 ================= */
static SDL_Window   *g_window   = NULL;
static SDL_Renderer *g_renderer = NULL;
static SDL_Texture  *g_texture  = NULL;
static bool running = true;

/* ================= 输入状态 ================= */
static int16_t mouse_x = 0;
static int16_t mouse_y = 0;
static bool mouse_pressed = false;


/* =========================================================
 * SDL 初始化
 * ========================================================= */
int sdl_init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return -1;
    }

    g_window = SDL_CreateWindow(
        "LVGL + SDL3 Demo",
        WIDTH, HEIGHT,
        SDL_WINDOW_HIGH_PIXEL_DENSITY
    );

    g_renderer = SDL_CreateRenderer(g_window, NULL);
    g_texture  = SDL_CreateTexture(
        g_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    if (!g_window || !g_renderer || !g_texture) {
        SDL_Log("SDL create failed: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

/* =========================================================
 * LVGL 显示刷新
 * ========================================================= */
void disp_flush(
    lv_display_t *disp,
    const lv_area_t *area,
    uint8_t *px_map
)
{
    SDL_Rect rect = {
        .x = area->x1,
        .y = area->y1,
        .w = area->x2 - area->x1 + 1,
        .h = area->y2 - area->y1 + 1
    };

    int pitch = rect.w * BYTE_PER_PIXEL;

    SDL_UpdateTexture(
        g_texture,
        &rect,
        px_map,
        pitch
    );

    lv_display_flush_ready(disp);
}


/* =========================================================
 * LVGL 显示设备初始化
 * ========================================================= */
void lv_port_disp_init(void)
{
    lv_display_t *disp = lv_display_create(WIDTH, HEIGHT);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_ARGB8888);
    lv_display_set_flush_cb(disp, disp_flush);

    static uint8_t buf1[WIDTH * BUF_HEIGHT * BYTE_PER_PIXEL];
    static uint8_t buf2[WIDTH * BUF_HEIGHT * BYTE_PER_PIXEL];

    lv_display_set_buffers(
        disp,
        buf1,
        buf2,
        sizeof(buf1),
        LV_DISPLAY_RENDER_MODE_PARTIAL
    );
}

/* =========================================================
 * LVGL 输入设备（鼠标）
 * ========================================================= */
void mouse_read_cb(
    lv_indev_t *indev,
    lv_indev_data_t *data
)
{
    data->point.x = mouse_x;
    data->point.y = mouse_y;
    data->state   = mouse_pressed
                        ? LV_INDEV_STATE_PRESSED
                        : LV_INDEV_STATE_RELEASED;
}

void lv_port_input_init(void)
{
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, mouse_read_cb);
}

/* =========================================================
 * SDL 事件处理
 * ========================================================= */
void sdl_handle_event(SDL_Event *e)
{
    switch (e->type) {
    case SDL_EVENT_QUIT:
        running = false;
        break;

    case SDL_EVENT_MOUSE_MOTION:
        mouse_x = e->motion.x;
        mouse_y = e->motion.y;
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        mouse_pressed = true;
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        mouse_pressed = false;
        break;

    default:
        break;
    }
}


void sdl_handler_loop()
{
    
    uint32_t last_tick = SDL_GetTicks();
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            sdl_handle_event(&e);
        }
    
        uint32_t now = SDL_GetTicks();
        lv_tick_inc(now - last_tick);
        last_tick = now;
    
        lv_timer_handler();
    
        SDL_RenderClear(g_renderer);
        SDL_RenderTexture(g_renderer, g_texture, NULL, NULL);
        SDL_RenderPresent(g_renderer);
    
        SDL_Delay(1);
    }

    SDL_DestroyTexture(g_texture);
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}