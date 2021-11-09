//
// Created by WSJ on 2021/11/5.
//

#include "UI_task.h"
#include "UI.h"

#define UI_TASK_INIT_TIME 350

void UI_task(void *pvParameters) {
    //空闲一段时间 等待系统初始化
    vTaskDelay(UI_TASK_INIT_TIME);

    UI_Line l1(UI_Graph_ADD, 9, 200, 200, 300, 300);

    while(1)
    {
        l1.Refresh();
        vTaskDelay(100);
    }




}