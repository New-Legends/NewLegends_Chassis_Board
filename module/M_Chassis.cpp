//
// Created by WSJ on 2021/11/2.
//

#include <first_order_filter.h>
#include "M_Chassis.h"
#include "user_lib.h"
#include <cmath>
#include "CAN_receive.h"

//#define CHASSIS_NO_CURRENT //测试时关闭底盘电源


void M_Chassis::init() {
    //底盘速度环pid值
    const static fp32 motor_speed_pid[3] = {M3505_MOTOR_SPEED_PID_KP, M3505_MOTOR_SPEED_PID_KI,
                                            M3505_MOTOR_SPEED_PID_KD};

    //底盘角度pid值
    const static fp32 chassis_yaw_pid[3] = {CHASSIS_FOLLOW_GIMBAL_PID_KP, CHASSIS_FOLLOW_GIMBAL_PID_KI,
                                            CHASSIS_FOLLOW_GIMBAL_PID_KD};

    const static fp32 chassis_x_order_filter[1] = {CHASSIS_ACCEL_X_NUM};
    const static fp32 chassis_y_order_filter[1] = {CHASSIS_ACCEL_Y_NUM};
    uint8_t i;

    //底盘开机状态为原始
    chassis_mode = CHASSIS_VECTOR_RAW;
    Rc.data = &rc_ctrl;
    //TODO:和柯杰对接上下板↓
//  获取陀螺仪姿态角指针
//  chassis_INS_angle = get_INS_angle_point();
//  获取云台电机数据指针
//  chassis_yaw_motor = get_yaw_motor_point();
//  chassis_pitch_motor = get_pitch_motor_point();


    //get chassis motor data point,  initialize motor speed PID
    //获取底盘电机数据指针，初始化PID
    for (i = 0; i < 4; i++) {
        motor_chassis[i].chassis_motor_measure = Chassis_Can->get_chassis_motor_measure_point(i);
        chassis_speed_pid[i].PID_init(PID_POSITION, motor_speed_pid, M3505_MOTOR_SPEED_PID_MAX_OUT,
                                      M3505_MOTOR_SPEED_PID_MAX_IOUT);
    }
    //初始化角度PID
    chassis_angle_pid.PID_init(PID_POSITION, chassis_yaw_pid, CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT,
                               CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT);

    //用一阶滤波代替斜波函数生成
    chassis_cmd_slow_set_vx.first_order_filter_init(CHASSIS_CONTROL_TIME, chassis_x_order_filter);
    chassis_cmd_slow_set_vy.first_order_filter_init(CHASSIS_CONTROL_TIME, chassis_y_order_filter);

    //最大 最小速度
    vx_max_speed = NORMAL_MAX_CHASSIS_SPEED_X;
    vx_min_speed = -NORMAL_MAX_CHASSIS_SPEED_X;

    vy_max_speed = NORMAL_MAX_CHASSIS_SPEED_Y;
    vy_min_speed = -NORMAL_MAX_CHASSIS_SPEED_Y;

    //更新一下数据
    feedback_update();
}

void M_Chassis::feedback_update() {
    uint8_t i = 0;
    for (i = 0; i < 4; i++) {
        //更新电机速度，加速度是速度的PID微分
        motor_chassis[i].speed = CHASSIS_MOTOR_RPM_TO_VECTOR_SEN *
                                 motor_chassis[i].chassis_motor_measure->speed_rpm;
        motor_chassis[i].accel =
                chassis_speed_pid[i].Dbuf[0] * CHASSIS_CONTROL_FREQUENCE;
    }

    //更新底盘纵向速度 x， 平移速度y，旋转速度wz，坐标系为右手系
    vx = (-motor_chassis[0].speed + motor_chassis[1].speed +
          motor_chassis[2].speed - motor_chassis[3].speed) *
         MOTOR_SPEED_TO_CHASSIS_SPEED_VX;
    vy = (-motor_chassis[0].speed - motor_chassis[1].speed +
          motor_chassis[2].speed + motor_chassis[3].speed) *
         MOTOR_SPEED_TO_CHASSIS_SPEED_VY;
    wz = (-motor_chassis[0].speed - motor_chassis[1].speed -
          motor_chassis[2].speed - motor_chassis[3].speed) *
         MOTOR_SPEED_TO_CHASSIS_SPEED_WZ / MOTOR_DISTANCE_TO_CENTER;

//TODO:计算底盘姿态角度, 如果底盘上有陀螺仪请更改这部分代码
//chassis_yaw = rad_format(*(chassis_INS_angle + INS_YAW_ADDRESS_OFFSET) -
//                         chassis_yaw_motor->relative_angle);
//chassis_pitch = rad_format(
//        *(chassis_INS_angle + INS_PITCH_ADDRESS_OFFSET) -
//        chassis_pitch_motor->relative_angle);
//chassis_roll = *(chassis_INS_angle + INS_ROLL_ADDRESS_OFFSET);

}

void M_Chassis::set_mode() {

    //遥控器设置模式
    if (switch_is_mid(Rc.data->rc.s[CHASSIS_MODE_CHANNEL])) {
        chassis_behaviour_mode = CHASSIS_NO_FOLLOW_YAW;
    } else if (switch_is_down(Rc.data->rc.s[CHASSIS_MODE_CHANNEL])) {
        chassis_behaviour_mode = CHASSIS_NO_MOVE;
    } else if (switch_is_up(Rc.data->rc.s[CHASSIS_MODE_CHANNEL])) {
        chassis_behaviour_mode = CHASSIS_ENGINEER_FOLLOW_CHASSIS_YAW;
    }

    //TODO:当云台在某些模式下，像初始化， 底盘不动
    //if (gimbal_cmd_to_chassis_stop()) {
    //    chassis_behaviour_mode = CHASSIS_NO_MOVE;
    //}


    //添加自己的逻辑判断进入新模式
    //根据行为模式选择一个底盘控制模式
    switch (chassis_behaviour_mode) {
        case CHASSIS_ZERO_FORCE:
            chassis_mode = CHASSIS_VECTOR_RAW;
            break;
        case CHASSIS_NO_MOVE:
            chassis_mode = CHASSIS_VECTOR_NO_FOLLOW_YAW;
            break;
        case CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW:
            chassis_mode = CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW;
            break;
        case CHASSIS_ENGINEER_FOLLOW_CHASSIS_YAW:
            chassis_mode = CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW;
            break;
        case CHASSIS_NO_FOLLOW_YAW:
            chassis_mode = CHASSIS_VECTOR_NO_FOLLOW_YAW;
            break;
        case CHASSIS_OPEN:
            chassis_mode = CHASSIS_VECTOR_RAW;
            break;
    }
}

void M_Chassis::mode_change_save() {
    if (last_chassis_mode == chassis_mode) {
        return;
    }
    //切入跟随云台模式
    if ((last_chassis_mode != CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW) &&
        chassis_mode == CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW) {
        chassis_relative_angle_set = 0.0f;
    }
        //切入跟随底盘角度模式
    else if ((last_chassis_mode != CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW) &&
             chassis_mode == CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW) {
        chassis_yaw_set = chassis_yaw;
    }
        //切入不跟随云台模式
    else if ((last_chassis_mode != CHASSIS_VECTOR_NO_FOLLOW_YAW) &&
             chassis_mode == CHASSIS_VECTOR_NO_FOLLOW_YAW) {
        chassis_yaw_set = chassis_yaw;
    }

    last_chassis_mode = chassis_mode;
}

void M_Chassis::set_contorl() {

    fp32 vx_set_ = 0.0f, vy_set_ = 0.0f, angle_set = 0.0f;
    //获取三个控制设置值
    chassis_behaviour_control_set(&vx_set_, &vy_set_, &angle_set);

    //跟随云台模式
    if (chassis_mode == CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW) {
        fp32 sin_yaw = 0.0f, cos_yaw = 0.0f;
        //旋转控制底盘速度方向，保证前进方向是云台方向，有利于运动平稳
        sin_yaw = sin(double(-chassis_yaw_motor->relative_angle));
        cos_yaw = cos(double(-chassis_yaw_motor->relative_angle));
        vx_set = cos_yaw * vx_set_ + sin_yaw * vy_set_;
        vy_set = -sin_yaw * vx_set_ + cos_yaw * vy_set_;
        //设置控制相对云台角度
        chassis_relative_angle_set = rad_format(angle_set);
        //计算旋转PID角速度
        wz_set = -chassis_angle_pid.PID_calc(
                chassis_yaw_motor->relative_angle,
                chassis_relative_angle_set);
        //速度限幅
        vx_set = fp32_constrain(vx_set_, vx_min_speed,
                                vx_max_speed);
        vy_set = fp32_constrain(vy_set_, vy_min_speed,
                                vy_max_speed);
    } else if (chassis_mode == CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW) {
        fp32 delat_angle = 0.0f;
        //设置底盘控制的角度
        chassis_yaw_set = rad_format(angle_set);
        delat_angle = rad_format(chassis_yaw_set - chassis_yaw);
        //计算旋转的角速度
        wz_set = chassis_angle_pid.PID_calc(0.0f, delat_angle);
        //速度限幅
        vx_set = fp32_constrain(vx_set_, vx_min_speed,
                                vx_max_speed);
        vy_set = fp32_constrain(vy_set_, vy_min_speed,
                                vy_max_speed);
    } else if (chassis_mode == CHASSIS_VECTOR_NO_FOLLOW_YAW) {
        //“angle_set” 是旋转速度控制
        wz_set = angle_set;
        vx_set = fp32_constrain(vx_set_, vx_min_speed,
                                vx_max_speed);
        vy_set = fp32_constrain(vy_set_, vy_min_speed,
                                vy_max_speed);
    } else if (chassis_mode == CHASSIS_VECTOR_RAW) {
        //在原始模式，设置值是发送到CAN总线
        vx_set = vx_set_;
        vy_set = vy_set_;
        wz_set = angle_set;
        chassis_cmd_slow_set_vx.out = 0.0f;
        chassis_cmd_slow_set_vy.out = 0.0f;
    }
}

void M_Chassis::chassis_behaviour_control_set(fp32 *vx_set_, fp32 *vy_set_, fp32 *angle_set) {
    if (vx_set_ == nullptr || vy_set_ == nullptr || angle_set == nullptr) {
        return;
    }
    switch (chassis_behaviour_mode) {
        case CHASSIS_ZERO_FORCE:
            chassis_zero_force_control(vx_set_, vy_set_, angle_set);
            break;
        case CHASSIS_NO_MOVE:
            chassis_no_move_control(vx_set_, vy_set_, angle_set);
            break;
        case CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW:
            chassis_infantry_follow_gimbal_yaw_control(vx_set_, vy_set_, angle_set);
            break;
        case CHASSIS_ENGINEER_FOLLOW_CHASSIS_YAW:
            chassis_engineer_follow_chassis_yaw_control(vx_set_, vy_set_, angle_set);
            break;
        case CHASSIS_NO_FOLLOW_YAW:
            chassis_no_follow_yaw_control(vx_set_, vy_set_, angle_set);
            break;
        case CHASSIS_OPEN:
            chassis_open_set_control(vx_set_, vy_set_, angle_set);
            break;
    }
}

/**
  * @brief          底盘无力的行为状态机下，底盘模式是raw，故而设定值会直接发送到can总线上故而将设定值都设置为0
  * @author         RM
  * @param[in]      vx_set前进的速度 设定值将直接发送到can总线上
  * @param[in]      vy_set左右的速度 设定值将直接发送到can总线上
  * @param[in]      wz_set旋转的速度 设定值将直接发送到can总线上
  * @param[in]      chassis_move_rc_to_vector底盘数据
  * @retval         返回空
  */

void M_Chassis::chassis_zero_force_control(fp32 *vx_can_set, fp32 *vy_can_set, fp32 *wz_can_set) {
    if (vx_can_set == nullptr || vy_can_set == nullptr || wz_can_set == nullptr) {
        return;
    }
    *vx_can_set = 0.0f;
    *vy_can_set = 0.0f;
    *wz_can_set = 0.0f;
}

/**
  * @brief          底盘不移动的行为状态机下，底盘模式是不跟随角度，
  * @author         RM
  * @param[in]      vx_set前进的速度,正值 前进速度， 负值 后退速度
  * @param[in]      vy_set左右的速度,正值 左移速度， 负值 右移速度
  * @param[in]      wz_set旋转的速度，旋转速度是控制底盘的底盘角速度
  * @param[in]      chassis_move_rc_to_vector底盘数据
  * @retval         返回空
  */

void M_Chassis::chassis_no_move_control(fp32 *vx_set_, fp32 *vy_set_, fp32 *wz_set_) {
    if (vx_set_ == nullptr || vy_set_ == nullptr || wz_set_ == nullptr) {
        return;
    }
    *vx_set_ = 0.0f;
    *vy_set_ = 0.0f;
    *wz_set_ = 0.0f;
}

void M_Chassis::chassis_infantry_follow_gimbal_yaw_control(fp32 *vx_set_, fp32 *vy_set_, fp32 *angle_set) {

}

void M_Chassis::chassis_engineer_follow_chassis_yaw_control(fp32 *vx_set_, fp32 *vy_set_, fp32 *angle_set) {
    if (vx_set_ == nullptr || vy_set_ == nullptr || angle_set == nullptr) {
        return;
    }

    chassis_rc_to_control_vector(vx_set_, vy_set_);

    *angle_set = rad_format(chassis_yaw_set - CHASSIS_ANGLE_Z_RC_SEN * Rc.data->rc.ch[CHASSIS_WZ_CHANNEL]);
}


void M_Chassis::chassis_rc_to_control_vector(fp32 *vx_set_, fp32 *vy_set_) {
    if (vx_set_ == nullptr || vy_set_ == nullptr) {
        return;
    }

    int16_t vx_channel, vy_channel;
    fp32 vx_set_channel, vy_set_channel;

    //死区限制，因为遥控器可能存在差异 摇杆在中间，其值不为0
    rc_deadband_limit(Rc.data->rc.ch[CHASSIS_X_CHANNEL], vx_channel, CHASSIS_RC_DEADLINE);
    rc_deadband_limit(Rc.data->rc.ch[CHASSIS_Y_CHANNEL], vy_channel, CHASSIS_RC_DEADLINE);

    vx_set_channel = vx_channel * CHASSIS_VX_RC_SEN;
    vy_set_channel = vy_channel * -CHASSIS_VY_RC_SEN;

    //键盘控制
    if (Rc.data->key.v & CHASSIS_FRONT_KEY) {
        vx_set_channel = vx_max_speed;
    } else if (Rc.data->key.v & CHASSIS_BACK_KEY) {
        vx_set_channel = vx_min_speed;
    }

    if (Rc.data->key.v & CHASSIS_LEFT_KEY) {
        vy_set_channel = vy_max_speed;
    } else if (Rc.data->key.v & CHASSIS_RIGHT_KEY) {
        vy_set_channel = vy_min_speed;
    }

    //一阶低通滤波代替斜波作为底盘速度输入
    chassis_cmd_slow_set_vx.first_order_filter_cali(vx_set_channel);
    chassis_cmd_slow_set_vy.first_order_filter_cali(vy_set_channel);
    //停止信号，不需要缓慢加速，直接减速到零
    if (vx_set_channel < CHASSIS_RC_DEADLINE * CHASSIS_VX_RC_SEN &&
        vx_set_channel > -CHASSIS_RC_DEADLINE * CHASSIS_VX_RC_SEN) {
        chassis_cmd_slow_set_vx.out = 0.0f;
    }

    if (vy_set_channel < CHASSIS_RC_DEADLINE * CHASSIS_VY_RC_SEN &&
        vy_set_channel > -CHASSIS_RC_DEADLINE * CHASSIS_VY_RC_SEN) {
        chassis_cmd_slow_set_vy.out = 0.0f;
    }

    *vx_set_ = chassis_cmd_slow_set_vx.out;
    *vy_set_ = chassis_cmd_slow_set_vy.out;
}

void M_Chassis::chassis_no_follow_yaw_control(fp32 *vx_set_, fp32 *vy_set_, fp32 *wz_set_) {
    if (vx_set_ == nullptr || vy_set_ == nullptr || wz_set_ == nullptr) {
        return;
    }

    chassis_rc_to_control_vector(vx_set_, vy_set_);
    *wz_set_ = -CHASSIS_WZ_RC_SEN * Rc.data->rc.ch[CHASSIS_WZ_CHANNEL];

}

void M_Chassis::chassis_open_set_control(fp32 *vx_set_, fp32 *vy_set_, fp32 *wz_set_) {
    if (vx_set_ == nullptr || vy_set_ == nullptr || wz_set_ == nullptr) {
        return;
    }

    *vx_set_ = Rc.data->rc.ch[CHASSIS_X_CHANNEL] * CHASSIS_OPEN_RC_SCALE;
    *vy_set_ = -Rc.data->rc.ch[CHASSIS_Y_CHANNEL] * CHASSIS_OPEN_RC_SCALE;
    *wz_set_ = -Rc.data->rc.ch[CHASSIS_WZ_CHANNEL] * CHASSIS_OPEN_RC_SCALE;
    return;
}

void M_Chassis::solve() {
    fp32 max_vector = 0.0f, vector_rate = 0.0f;
    fp32 temp = 0.0f;
    fp32 wheel_speed[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    uint8_t i = 0;

    //旋转的时候， 由于云台靠前，所以是前面两轮 0 ，1 旋转的速度变慢， 后面两轮 2,3 旋转的速度变快
    wheel_speed[0] = -vx_set - vy_set + (CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[1] = vx_set - vy_set + (CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[2] = vx_set + vy_set + (-CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[3] = -vx_set + vy_set + (-CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;

    if (chassis_mode == CHASSIS_VECTOR_RAW) {
        for (i = 0; i < 4; i++) {
            motor_chassis[i].give_current = (int16_t) (wheel_speed[i]);
        }
        //raw控制直接返回
        return;
    }
    //计算轮子控制最大速度，并限制其最大速度
    for (i = 0; i < 4; i++) {
        motor_chassis[i].speed_set = wheel_speed[i];
        temp = fabs(motor_chassis[i].speed_set);
        if (max_vector < temp) {
            max_vector = temp;
        }
    }

    if (max_vector > MAX_WHEEL_SPEED) {
        vector_rate = MAX_WHEEL_SPEED / max_vector;
        for (i = 0; i < 4; i++) {
            motor_chassis[i].speed_set *= vector_rate;
        }
    }

}

void M_Chassis::pid_calc() {
    for (int i = 0; i < 4; i++) {
        chassis_speed_pid[i].PID_calc(motor_chassis[i].speed, motor_chassis[i].speed_set);
    }
}

void M_Chassis::power_ctrl() {

}

void M_Chassis::go() {
    for(int i = 0; i < 4; i++) {
#ifdef CHASSIS_NO_CURRENT
        motor_chassis[i].give_current = 0;
#else
        motor_chassis[i].give_current = (int16_t) (chassis_speed_pid[i].out);
#endif
    }
    Chassis_Can->CAN_cmd_chassis(motor_chassis[0].give_current, motor_chassis[1].give_current,
                        motor_chassis[2].give_current, motor_chassis[3].give_current);
}


