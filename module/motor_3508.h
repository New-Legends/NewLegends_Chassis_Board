//
// Created by WSJ on 2021/11/2.
//

#ifndef CLASSIS_BOARD_MOTOR_3508_H
#define CLASSIS_BOARD_MOTOR_3508_H

#ifdef __cplusplus
#include "motor_measure.h"
//m3508转化成底盘速度(m/s)的比例，
#define M3508_MOTOR_RPM_TO_VECTOR 0.000415809748903494517209f
#define CHASSIS_MOTOR_RPM_TO_VECTOR_SEN M3508_MOTOR_RPM_TO_VECTOR

//单个底盘电机最大速度
#define MAX_3508_SPEED 4.0f

class motor_3508 {
public:
    const motor_measure *chassis_motor_measure;
    fp32 accel;
    fp32 speed;
    fp32 speed_set;
    int16_t give_current;
};

#endif

#endif //CLASSIS_BOARD_MOTOR_3508_H
