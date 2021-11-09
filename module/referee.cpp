//
// Created by WSJ on 2021/11/4.
//

#include "referee.h"

referee Referee;

referee::referee() {
    Robot_ID = &robot_state.robot_id;//获取机器人ID
    Camp = judge_camp();//获取机器人阵营
    Robot_Client_ID = get_client_id();
    RECEIVER_ID = Robot_Client_ID;
}

robot_camp referee::judge_camp() const {
    if (*Robot_ID > 10)
        return BLUE;
    else
        return RED;
}

uint16_t referee::get_client_id() const {
    if (Camp == BLUE) {
        return 0x0110 + (*Robot_ID - 0x10);//计算客户端ID
    } else {
        return 0x0100 + *Robot_ID;//计算客户端ID
    }
}

float referee::get_chassis_power() const {
    return Power_heat->chassis_power;
}

uint16_t referee::get_chassis_power_buffer() const {
    return Power_heat->chassis_power_buffer;
}

uint16_t referee::get_chassis_power_limit() const {
    return Robot_state->chassis_power_limit;
}
