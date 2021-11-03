//
// Created by WSJ on 2021/11/2.
//

#ifndef CLASSIS_BOARD_MOTOR_6020_H
#define CLASSIS_BOARD_MOTOR_6020_H
#ifdef __cplusplus
#include "pid.h"
#include "motor_measure.h"
#include "struct_typedef.h"


typedef enum {
    GIMBAL_MOTOR_RAW = 0, //电机原始值控制
    GIMBAL_MOTOR_GYRO,    //电机陀螺仪角度控制
    GIMBAL_MOTOR_ENCONDE, //电机编码值角度控制
} gimbal_motor_mode_e;

class motor_6020 {
public:
    const motor_measure *gimbal_motor_measure;
    pid gimbal_motor_absolute_angle_pid;
    pid gimbal_motor_relative_angle_pid;
    pid gimbal_motor_gyro_pid;
    gimbal_motor_mode_e gimbal_motor_mode;
    gimbal_motor_mode_e last_gimbal_motor_mode;
    uint16_t offset_ecd;
    fp32 max_relative_angle; //rad
    fp32 min_relative_angle; //rad

    fp32 relative_angle;     //rad
    fp32 relative_angle_set; //rad
    fp32 absolute_angle;     //rad
    fp32 absolute_angle_set; //rad
    fp32 motor_gyro;         //rad/s
    fp32 motor_gyro_set;
    fp32 motor_speed;
    fp32 raw_cmd_current;
    fp32 current_set;
    int16_t given_current;
};
#endif

#endif //CLASSIS_BOARD_MOTOR_6020_H
