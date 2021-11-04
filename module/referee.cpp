//
// Created by WSJ on 2021/11/4.
//

#include "referee.h"

referee::referee() {
    Robot_ID = robot_state.robot_id;//获取机器人ID
    Camp = judge_camp();//获取机器人阵营
}

robot_camp referee::judge_camp() const {
    if (Robot_ID > 10)
        return BLUE;
    else
        return RED;
}
