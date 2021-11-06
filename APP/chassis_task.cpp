//
// Created by WSJ on 2021/11/2.
//

#include "chassis_task.h"
#include "M_Chassis.h"
#include "referee.h"

//任务开始空闲一段时间
#define CHASSIS_TASK_INIT_TIME 357

void chassis_task(void *pvParameters) {
    //空闲一段时间 等待系统初始化
    vTaskDelay(CHASSIS_TASK_INIT_TIME);
    M_Chassis chassis{};
    chassis.init();
    while (1) {
        chassis.set_mode();
        chassis.mode_change_save();
        chassis.feedback_update();
        chassis.set_contorl();
        chassis.solve();
        chassis.pid_calc();
        chassis.power_ctrl();
        chassis.go();
    }

}