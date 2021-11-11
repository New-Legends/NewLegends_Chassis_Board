//
// Created by WSJ on 2021/11/5.
//

#ifndef CPROJECT_SUPER_CAP_H
#define CPROJECT_SUPER_CAP_H

#include "main.h"
#include "stdbool.h"
#include "CAN_receive.h"


class SuperCap {
public:

    float input_vot;//输入电压
    float super_cap_vot;//超级电容电压
    float input_current;//输入电流
    float target_power;//目标功率
    bool cap_change = false; //超电电压过低标识符
    static void Init();

    static void CAN1_Cap_Send(uint16_t temPower);
};


#endif //CPROJECT_SUPER_CAP_H
