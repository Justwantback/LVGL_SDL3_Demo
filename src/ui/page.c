#include "lvgl.h"
#include "comps.h"
#include "widget.h"
#include "component/statusbar.h"
#include <stdio.h>

/* 测试页面 */
void ui_page_create_test(lv_obj_t *parent)
{
    /* 1. 创建状态栏 Component */
    ui_component_t *statusbar = ui_statusbar_create(parent);
    
    /* 2. 模拟事件分发 */
    ui_event_param_time_t time_param = {12, 30};
    ui_component_dispatch_event(statusbar, UI_EVENT_TIME_UPDATE, &time_param);

    /* 3. 模拟触发器检测 (Trigger Check) */
    /* 假设这是来自底层的 RSSI 信号强度原始值 */
    int rssi_raw = -90; 
    
    /* 调用 check_triggers，内部会：
       1. 找到 wifi_widget
       2. 执行 trigger_wifi_check(rssi_raw)
       3. trigger 发现信号太弱，返回 UI_EVENT_WIFI_STATUS
       4. 自动 process 该事件，更新图标为 0 格
    */
    ui_component_check_triggers(statusbar, &rssi_raw);
}
