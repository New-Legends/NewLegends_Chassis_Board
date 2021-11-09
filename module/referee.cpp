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

robot_camp referee::judge_camp() {
    if (*Robot_ID > 10)
        return BLUE;
    else
        return RED;
}

uint16_t referee::get_client_id() {
    if (Camp == BLUE) {
        return 0x0110 + (*Robot_ID - 0x10);//计算客户端ID
    } else {
        return 0x0100 + *Robot_ID;//计算客户端ID
    }
}