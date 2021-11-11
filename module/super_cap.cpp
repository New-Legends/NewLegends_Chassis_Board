//
// Created by WSJ on 2021/11/5.
//

#include "super_cap.h"
#include "stm32f4xx.h"
#include "super_cap.h"
#include "can.h"
#include "remote_control.h"
#include "led.h"
#include "CAN_receive.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

SuperCap Cap;
static CAN_TxHeaderTypeDef cap_tx_message;
static uint8_t cap_can_send_data[8] = {0};

void SuperCap::CAN1_Cap_Send(uint16_t temPower) {
    uint32_t send_mail_box;
    cap_tx_message.StdId = 0x210;
    cap_tx_message.IDE = CAN_ID_STD; //0x0000
    cap_tx_message.RTR = CAN_RTR_DATA; //0x0000
    cap_tx_message.DLC = 0x08;

    cap_can_send_data[0] = (temPower >> 8);
    cap_can_send_data[1] = (temPower);
    cap_can_send_data[2] = 0;
    cap_can_send_data[3] = 0;
    cap_can_send_data[4] = 0;
    cap_can_send_data[5] = 0;
    cap_can_send_data[6] = 0;
    cap_can_send_data[7] = 0;

    HAL_CAN_AddTxMessage(&hcan1, &cap_tx_message, cap_can_send_data, &send_mail_box);
}

void SuperCap::Init() {
    CAN1_Cap_Send(7500);
}
