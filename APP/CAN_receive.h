//
// Created by WSJ on 2021/11/2.
//

#ifndef CLASSIS_BOARD_CAN_RECEIVE_H
#define CLASSIS_BOARD_CAN_RECEIVE_H
#ifdef __cplusplus

#include <motor_measure.h>
#include "struct_typedef.h"
#include "main.h"



#define CHASSIS_CAN hcan1

typedef enum {
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204,

    CAN_SUPER_CAP_ID = 0x211,
} can_msg_id_e;

class CAN_receive {
public:
    CAN_TxHeaderTypeDef chassis_tx_message;
    uint8_t chassis_can_send_data[8];
    motor_measure motor_chassis[4];
    const motor_measure *get_chassis_motor_measure_point(uint8_t i);

    void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

    void CAN_cmd_chassis_reset_ID();

};

#endif
#endif //CLASSIS_BOARD_CAN_RECEIVE_H
