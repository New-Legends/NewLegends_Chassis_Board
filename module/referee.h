//
// Created by WSJ on 2021/11/4.
//

#ifndef CPROJECT_REFEREE_H
#define CPROJECT_REFEREE_H

#include "struct_typedef.h"
#include "protocol.h"
#include "referee_data.h"

extern frame_header_struct_t referee_receive_header;
extern frame_header_struct_t referee_send_header;

extern ext_game_state_t game_state;
extern ext_game_result_t game_result;
extern ext_game_robot_HP_t game_robot_HP_t;

extern ext_event_data_t field_event;
extern ext_supply_projectile_action_t supply_projectile_action_t;
extern ext_supply_projectile_booking_t supply_projectile_booking_t;
extern ext_referee_warning_t referee_warning_t;
extern ext_dart_remaining_time_t dart_remaining_time_t;

extern ext_game_robot_state_t robot_state;
extern ext_power_heat_data_t power_heat_data_t;
extern ext_game_robot_pos_t game_robot_pos_t;
extern ext_buff_musk_t buff_musk_t;
extern aerial_robot_energy_t aerial_robot_energy;
extern ext_robot_hurt_t robot_hurt_t;
extern ext_shoot_data_t shoot_data_t;
extern ext_bullet_remaining_t bullet_remaining_t;
extern ext_rfid_status_t rfid_status_t;
extern ext_dart_client_cmd_t dart_client_cmd;
extern ext_student_interactive_data_t student_interactive_data_t;

static uint16_t RECEIVER_ID;
class referee {
public:
    const frame_header_struct_t *Receive_header = &referee_receive_header;//接受报文
    const frame_header_struct_t *Send_header = &referee_send_header;//发送报文

    const ext_game_state_t *Game_state = &game_state;//比赛状态
    const ext_game_result_t *Game_result = &game_result;//比赛结果
    const ext_game_robot_HP_t *Game_robot_HP = &game_robot_HP_t;//场上机器人的血量

    const ext_event_data_t *Field_data = &field_event;//场地事件数据
    const ext_supply_projectile_action_t *Supply_action = &supply_projectile_action_t;//补给站动作标识数据
    //const ext_supply_projectile_booking_t supply_projectile_booking_t;
    const ext_referee_warning_t *Referee_warning = &referee_warning_t;//裁判警告信息  己方警告发生后发送
    const ext_dart_remaining_time_t *Dart_remaning_time = &dart_remaining_time_t;//飞镖发射口倒计时


    const ext_game_robot_state_t *Robot_state = &robot_state;//比赛机器人状态
    const ext_power_heat_data_t *Power_heat = &power_heat_data_t;//实时功率热量数据
    const ext_game_robot_pos_t *Robot_posion = &game_robot_pos_t;//机器人位置
    const ext_buff_musk_t *Buff = &buff_musk_t;//机器人增益
    const aerial_robot_energy_t *Aerial_robot_energy = &aerial_robot_energy;//空中机器人能量状态(可攻击时间)
    const ext_robot_hurt_t *Robot_hurt = &robot_hurt_t;//伤害来源
    const ext_shoot_data_t *Shoot_data_t = &shoot_data_t;//实时射击信息
    const ext_bullet_remaining_t *Bullet_remain = &bullet_remaining_t;//子弹剩余发射数
    const ext_rfid_status_t *RFID_status = &rfid_status_t;//RFID状态
    const ext_dart_client_cmd_t *Dart_client_cmd = &dart_client_cmd;//飞镖发射架状态
    //const ext_student_interactive_data_t student_interactive_data_t;


    uint8_t *Robot_ID;

    uint16_t Robot_Client_ID;
    robot_camp Camp;

    [[nodiscard]] robot_camp judge_camp();

    [[nodiscard]] uint16_t get_client_id();

    referee();
};

#endif //CPROJECT_REFEREE_H
