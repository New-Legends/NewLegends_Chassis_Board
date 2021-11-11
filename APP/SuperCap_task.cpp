//
// Created by WSJ on 2021/11/11.
//


#include "SuperCap_task.h"
#include "super_cap.h"
#include "referee.h"

#define SUPER_CAP_CONTROL_TIME_MS 1

extern SuperCap Cap;
extern referee Referee;

void SuperCap_task(void *pvParameters) {

    SuperCap::Init();
    while (true) {
        if (Cap.super_cap_vot <= 14 || Cap.cap_change) //电容电压不要低于12V，这可能会造成C620低压保护,电机直接断电
        {
            if (Cap.super_cap_vot >= 20) {
                if (Referee.Robot_state->chassis_power_limit <= 40)//当前底盘最大功率限制<40  目标功率为39w
                {
                    SuperCap::CAN1_Cap_Send(3900);
                } else if (Referee.Robot_state->chassis_power_limit > 40 &&
                           Referee.Robot_state->chassis_power_limit <= 50)//当前底盘最大功率限制40-50w  目标功率为49w
                {
                    SuperCap::CAN1_Cap_Send(4900);
                } else if (Referee.Robot_state->chassis_power_limit > 50 &&
                           Referee.Robot_state->chassis_power_limit <= 60)//当前底盘最大功率限制50-60w  目标功率为59w
                {
                    SuperCap::CAN1_Cap_Send(5900);
                } else if (Referee.Robot_state->chassis_power_limit > 60 &&
                           Referee.Robot_state->chassis_power_limit <= 70)//当前底盘最大功率限制60-70w  目标功率为79w
                {
                    SuperCap::CAN1_Cap_Send(6900);
                } else if (Referee.Robot_state->chassis_power_limit > 70 &&
                           Referee.Robot_state->chassis_power_limit <= 80)//当前底盘最大功率限制70-80w  目标功率为79w
                {
                    SuperCap::CAN1_Cap_Send(7900);
                } else if (Referee.Robot_state->chassis_power_limit > 80 &&
                           Referee.Robot_state->chassis_power_limit <= 100)//当前底盘最大功率限制80-100w  目标功率为99w
                {
                    SuperCap::CAN1_Cap_Send(9900);
                } else if (Referee.Robot_state->chassis_power_limit > 100 &&
                           Referee.Robot_state->chassis_power_limit < 120)//当前底盘最大功率限制100-120w  目标功率为119w
                {
                    SuperCap::CAN1_Cap_Send(11900);
                }
                Cap.cap_change = false;
            } else {
                Cap.cap_change = true;
                SuperCap::CAN1_Cap_Send(13000);
            }
        } else {
            if (Referee.Robot_state->chassis_power_limit <= 40)//当前底盘最大功率限制<40  目标功率为39w
            {
                SuperCap::CAN1_Cap_Send(3900);
            } else if (Referee.Robot_state->chassis_power_limit > 40 &&
                       Referee.Robot_state->chassis_power_limit <= 50)//当前底盘最大功率限制40-50w  目标功率为49w
            {
                SuperCap::CAN1_Cap_Send(4900);
            } else if (Referee.Robot_state->chassis_power_limit > 50 &&
                       Referee.Robot_state->chassis_power_limit <= 60)//当前底盘最大功率限制50-60w  目标功率为59w
            {
                SuperCap::CAN1_Cap_Send(5900);
            } else if (Referee.Robot_state->chassis_power_limit > 60 &&
                       Referee.Robot_state->chassis_power_limit <= 70)//当前底盘最大功率限制60-70w  目标功率为79w
            {
                SuperCap::CAN1_Cap_Send(6900);
            } else if (Referee.Robot_state->chassis_power_limit > 70 &&
                       Referee.Robot_state->chassis_power_limit <= 80)//当前底盘最大功率限制70-80w  目标功率为79w
            {
                SuperCap::CAN1_Cap_Send(7900);
            } else if (Referee.Robot_state->chassis_power_limit > 80 &&
                       Referee.Robot_state->chassis_power_limit <= 100)//当前底盘最大功率限制80-100w  目标功率为99w
            {
                SuperCap::CAN1_Cap_Send(9900);
            } else if (Referee.Robot_state->chassis_power_limit > 100 &&
                       Referee.Robot_state->chassis_power_limit < 120)//当前底盘最大功率限制100-120w  目标功率为119w
            {
                SuperCap::CAN1_Cap_Send(11900);
            }
        }
        vTaskDelay(SUPER_CAP_CONTROL_TIME_MS);
    }
}
