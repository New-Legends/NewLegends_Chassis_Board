//
// Created by WSJ on 2021/11/2.
//

#ifndef CLASSIS_BOARD_M_CHASSIS_H
#define CLASSIS_BOARD_M_CHASSIS_H

#include "motor_3508.h"
#include "pid.h"
#include "first_order_filter.h"
#include "motor_6020.h"
#include "remote_control.h"
#include "CAN_receive.h"

//任务开始空闲一段时间
#define CHASSIS_TASK_INIT_TIME 357

/*  左上按钮        0
    右上按钮        1

    右边左右摇杆     0
    右边上下摇杆     1
    左边左右摇杆     2
    左边左右摇杆     3*/

//选择底盘状态 开关通道号
#define CHASSIS_MODE_CHANNEL 0
//前后的遥控器通道号码
#define CHASSIS_X_CHANNEL 3
//左右的遥控器通道号码
#define CHASSIS_Y_CHANNEL 2
//在特殊模式下，可以通过遥控器控制旋转
#define CHASSIS_WZ_CHANNEL 0

//遥控器前进摇杆（max 660）转化成车体前进速度（m/s）的比例
#define CHASSIS_VX_RC_SEN 0.0006f
//遥控器左右摇杆（max 660）转化成车体左右速度（m/s）的比例
#define CHASSIS_VY_RC_SEN 0.0005f
//跟随底盘yaw模式下，遥控器的yaw遥杆（max 660）增加到车体角度的比例
#define CHASSIS_ANGLE_Z_RC_SEN 0.0000002f
//不跟随云台的时候 遥控器的yaw遥杆（max 660）转化成车体旋转速度的比例
#define CHASSIS_WZ_RC_SEN 0.005f

#define CHASSIS_ACCEL_X_NUM 0.1666666667f
#define CHASSIS_ACCEL_Y_NUM 0.3333333333f

//摇杆死区
#define CHASSIS_RC_DEADLINE 10

#define MOTOR_SPEED_TO_CHASSIS_SPEED_VX 0.25f
#define MOTOR_SPEED_TO_CHASSIS_SPEED_VY 0.25f
#define MOTOR_SPEED_TO_CHASSIS_SPEED_WZ 0.25f


#define MOTOR_DISTANCE_TO_CENTER 0.2f

//底盘任务控制间隔 2ms
#define CHASSIS_CONTROL_TIME_MS 2
//底盘任务控制间隔 0.002s
#define CHASSIS_CONTROL_TIME 0.002f
//底盘任务控制频率，尚未使用这个宏
#define CHASSIS_CONTROL_FREQUENCE 500.0f
//底盘3508最大can发送电流值
#define MAX_MOTOR_CAN_CURRENT 16000.0f
//底盘摇摆按键
#define SWING_KEY KEY_PRESSED_OFFSET_CTRL
//底盘前后左右控制按键
#define CHASSIS_FRONT_KEY KEY_PRESSED_OFFSET_W
#define CHASSIS_BACK_KEY KEY_PRESSED_OFFSET_S
#define CHASSIS_LEFT_KEY KEY_PRESSED_OFFSET_A
#define CHASSIS_RIGHT_KEY KEY_PRESSED_OFFSET_D

//m3508转化成底盘速度(m/s)的比例，
#define M3508_MOTOR_RPM_TO_VECTOR 0.000415809748903494517209f
#define CHASSIS_MOTOR_RPM_TO_VECTOR_SEN M3508_MOTOR_RPM_TO_VECTOR

//单个底盘电机最大速度
#define MAX_WHEEL_SPEED 4.0f
//底盘运动过程最大前进速度
#define NORMAL_MAX_CHASSIS_SPEED_X 2.0f
//底盘运动过程最大平移速度
#define NORMAL_MAX_CHASSIS_SPEED_Y 1.5f

#define CHASSIS_WZ_SET_SCALE 0.1f

//摇摆原地不动摇摆最大角度(rad)
#define SWING_NO_MOVE_ANGLE 0.7f
//摇摆过程底盘运动最大角度(rad)
#define SWING_MOVE_ANGLE 0.31415926535897932384626433832795f

//chassis motor speed PID
//底盘电机速度环PID
#define M3505_MOTOR_SPEED_PID_KP 6000.0f
#define M3505_MOTOR_SPEED_PID_KI 0.0f
#define M3505_MOTOR_SPEED_PID_KD 2.0f
#define M3505_MOTOR_SPEED_PID_MAX_OUT  6000.0f
#define M3505_MOTOR_SPEED_PID_MAX_IOUT 2000.0f

//chassis follow angle PID
//底盘旋转跟随PID
#define CHASSIS_FOLLOW_GIMBAL_PID_KP 5.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_KI 0.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_KD 0.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT 6.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT 0.0f

#define POWER_LIMIT         80.0f
#define WARNING_POWER       40.0f
#define WARNING_POWER_BUFF  35.0f

#define NO_JUDGE_TOTAL_CURRENT_LIMIT    64000.0f    //16000 * 4,
#define BUFFER_TOTAL_CURRENT_LIMIT      16000.0f
#define POWER_TOTAL_CURRENT_LIMIT       20000.0f


typedef enum {
    CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW,   //底盘会跟随云台相对角度
    CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW,  //底盘有底盘角度控制闭环
    CHASSIS_VECTOR_NO_FOLLOW_YAW,       //底盘有旋转速度控制
    CHASSIS_VECTOR_RAW,                 //CAN直接发送电流

} chassis_mode_e;

typedef enum {
    CHASSIS_ZERO_FORCE,                   //底盘无力, 跟没上电那样
    CHASSIS_NO_MOVE,                      //底盘保持不动
    CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW,   //正常步兵底盘跟随云台
    CHASSIS_ENGINEER_FOLLOW_CHASSIS_YAW,  //工程底盘角度控制底盘，由于底盘未有陀螺仪，故而角度是减去云台角度而得到   如果有底盘陀螺仪请更新底盘的yaw，pitch，roll角度 在chassis_feedback_update函数中
    CHASSIS_NO_FOLLOW_YAW,                //底盘不跟随角度，角度是开环的，但轮子是有速度环
    CHASSIS_OPEN                          //遥控器的值乘以比例成电流值 直接发送到can总线上
} chassis_behaviour_e;
#define CHASSIS_OPEN_RC_SCALE 10 //在chassis_open 模型下，遥控器乘以该比例发送到can上
#ifdef __cplusplus
extern CAN_receive CAN_1;

class M_Chassis {
public:
    remote_control Rc;               //底盘使用的遥控器指针
    const motor_6020 *chassis_yaw_motor;   //底盘使用到yaw云台电机的相对角度来计算底盘的欧拉角.
    const motor_6020 *chassis_pitch_motor; //底盘使用到pitch云台电机的相对角度来计算底盘的欧拉角
    const fp32 *chassis_INS_angle;             //获取陀螺仪解算出的欧拉角指针
    chassis_mode_e chassis_mode;               //底盘控制状态机
    chassis_mode_e last_chassis_mode;          //底盘上次控制状态机
    chassis_behaviour_e chassis_behaviour_mode = CHASSIS_ZERO_FORCE;
    motor_3508 motor_chassis[4];          //底盘电机数据
    pid chassis_speed_pid[4];             //底盘电机速度pid
    pid chassis_angle_pid;              //底盘跟随角度pid

    first_order_filter chassis_cmd_slow_set_vx;  //使用一阶低通滤波减缓设定值
    first_order_filter chassis_cmd_slow_set_vy;  //使用一阶低通滤波减缓设定值



    fp32 vx;                          //底盘速度 前进方向 前为正，单位 m/s
    fp32 vy;                          //底盘速度 左右方向 左为正  单位 m/s
    fp32 wz;                          //底盘旋转角速度，逆时针为正 单位 rad/s
    fp32 vx_set;                      //底盘设定速度 前进方向 前为正，单位 m/s
    fp32 vy_set;                      //底盘设定速度 左右方向 左为正，单位 m/s
    fp32 wz_set;                      //底盘设定旋转角速度，逆时针为正 单位 rad/s
    fp32 chassis_relative_angle;      //底盘与云台的相对角度，单位 rad
    fp32 chassis_relative_angle_set;  //设置相对云台控制角度
    fp32 chassis_yaw_set;

    fp32 vx_max_speed;  //前进方向最大速度 单位m/s
    fp32 vx_min_speed;  //后退方向最大速度 单位m/s
    fp32 vy_max_speed;  //左方向最大速度 单位m/s
    fp32 vy_min_speed;  //右方向最大速度 单位m/s
    fp32 chassis_yaw;   //陀螺仪和云台电机叠加的yaw角度
    fp32 chassis_pitch; //陀螺仪和云台电机叠加的pitch角度
    fp32 chassis_roll;  //陀螺仪和云台电机叠加的roll角度

    CAN_receive *Chassis_Can = &CAN_1;

    void init();

    void feedback_update();

    void set_mode();

    void mode_change_save();

    void set_contorl();

    void solve();

    void pid_calc();

    void power_ctrl();

    void go();

    void chassis_behaviour_control_set(fp32 *vx_set_, fp32 *vy_set_, fp32 *angle_set);

    static void chassis_zero_force_control(fp32 *vx_can_set, fp32 *vy_can_set, fp32 *wz_can_set);

    static void chassis_no_move_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set);

    void chassis_infantry_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set);

    void chassis_engineer_follow_chassis_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set);

    void chassis_rc_to_control_vector(fp32 *vx_set_, fp32 *vy_set_);

    void chassis_no_follow_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set);

    void chassis_open_set_control(fp32 *vx_set_, fp32 *vy_set_, fp32 *wz_set_);


};

#endif

#endif //CLASSIS_BOARD_M_CHASSIS_H
