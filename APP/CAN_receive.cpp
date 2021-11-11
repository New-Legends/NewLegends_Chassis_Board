//
// Created by WSJ on 2021/11/2.
//

#include "CAN_receive.h"
#include "cmsis_os.h"
#include "super_cap.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

CAN_receive CAN_1;
extern SuperCap Cap;

const motor_measure *CAN_receive::get_chassis_motor_measure_point(uint8_t i) {
    return &motor_chassis[(i & 0x03)];
}

void CAN_receive::CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4) {
    uint32_t send_mail_box;
    chassis_tx_message.StdId = CAN_CHASSIS_ALL_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8;
    chassis_can_send_data[1] = motor1;
    chassis_can_send_data[2] = motor2 >> 8;
    chassis_can_send_data[3] = motor2;
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);

}

void CAN_receive::CAN_cmd_chassis_reset_ID() {
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x700;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = 0;
    chassis_can_send_data[1] = 0;
    chassis_can_send_data[2] = 0;
    chassis_can_send_data[3] = 0;
    chassis_can_send_data[4] = 0;
    chassis_can_send_data[5] = 0;
    chassis_can_send_data[6] = 0;
    chassis_can_send_data[7] = 0;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

    switch (rx_header.StdId) {
        case CAN_3508_M1_ID:
        case CAN_3508_M2_ID:
        case CAN_3508_M3_ID:
        case CAN_3508_M4_ID: {
            static uint8_t i = 0;
            i = rx_header.StdId - CAN_3508_M1_ID;
            CAN_1.motor_chassis[i].get_motor_measure(rx_data);
            // detect_hook(CHASSIS_MOTOR1_TOE + i);
            break;
        }
        case CAN_SUPER_CAP_ID:
            Cap.input_vot = (float) ((int16_t) (rx_data[1] << 8 | rx_data[0])) / 100.0f;  //输入电压
            Cap.super_cap_vot = ((float) ((int16_t) (rx_data[3] << 8 | rx_data[2])) / 100.0f);  //电容电压
            Cap.input_current = ((float) ((int16_t) (rx_data[5] << 8 | rx_data[4])) / 100.0f);      //输入电流
            Cap.target_power = ((float) ((int16_t) (rx_data[7] << 8 | rx_data[6])) / 100.0f);     //输入功率);
            break;
        default: {
            break;
        }
    }

}