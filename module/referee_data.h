//
// Created by WSJ on 2021/11/4.
//

#ifndef CPROJECT_REFEREE_DATA_H
#define CPROJECT_REFEREE_DATA_H

#include "main.h"

#define USART_RX_BUF_LENGHT     512
#define REFEREE_FIFO_BUF_LENGTH 1024

typedef enum {
    RED_HERO = 1,
    RED_ENGINEER = 2,
    RED_STANDARD_1 = 3,
    RED_STANDARD_2 = 4,
    RED_STANDARD_3 = 5,
    RED_AERIAL = 6,
    RED_SENTRY = 7,
    RED_RADAR = 9,
    BLUE_HERO = 101,
    BLUE_ENGINEER = 102,
    BLUE_STANDARD_1 = 103,
    BLUE_STANDARD_2 = 104,
    BLUE_STANDARD_3 = 105,
    BLUE_AERIAL = 106,
    BLUE_SENTRY = 107,
    BLUE_RADAR = 109,
} robot_id_t;
typedef enum {
    CLIENT_RED_HERO = 0x0101,
    CLIENT_RED_ENGINEER = 0x0202,
    CLIENT_RED_STANDARD_1 = 0x0103,
    CLIENT_RED_STANDARD_2 = 0x0104,
    CLIENT_RED_STANDARD_3 = 0x0105,
    CLIENT_RED_AERIAL = 0x0106,
    CLIENT_BLUE_HERO = 0x0165,
    CLIENT_BLUE_ENGINEER = 0x0166,
    CLIENT_BLUE_STANDARD_1 = 0x0167,
    CLIENT_BLUE_STANDARD_2 = 0x0168,
    CLIENT_BLUE_STANDARD_3 = 0x0169,
    CLIENT_BLUE_AERIAL = 0x016A,
} robot_client_id_t;
typedef enum{
    NONE,
    RED,
    BLUE,
}robot_camp;
typedef enum {
    PROGRESS_UNSTART = 0,
    PROGRESS_PREPARE = 1,
    PROGRESS_SELFCHECK = 2,
    PROGRESS_5sCOUNTDOWN = 3,
    PROGRESS_BATTLE = 4,
    PROGRESS_CALCULATING = 5,
} game_progress_t;
typedef struct //0001
{
    /*game_type:
         *1：RoboMaster 机甲大师赛
         *2：RoboMaster 机甲大师单项赛
         *3：ICRA RoboMaster 人工智能挑战赛
         *4：RoboMaster 联盟赛 3V3
         *5：RoboMaster 联盟赛 1V1
    */
    uint8_t game_type: 4;
    /*game_progress:
         *0：未开始比赛
         *1：准备阶段
         *2：自检阶段
         *3：5s 倒计时
         *4：对战中
         *5：比赛结算中
     */
    uint8_t game_progress: 4;
    /*stage_remain_time:
        *当前阶段剩余时间*/
    uint16_t stage_remain_time;
    /*SyncTimeStamp:
     * 机器人接收到该指令的精确 Unix 时间，当机载端收到有效的 NTP 服务器授时后生效*/
    uint64_t SyncTimeStamp;
}__attribute__((packed)) ext_game_state_t;

typedef struct //0002
{
    /*winner:
     * 0 平局 1 红方胜利 2 蓝方胜利
     */
    uint8_t winner;
}__attribute__((packed)) ext_game_result_t;
typedef struct {
    /*red_x_robot_HP:
     *红方x号机器人血量
     *1:英雄
     *2:工程
     *3:3号步兵
     *4:4号步兵
     *5:5号步兵
     *7:哨兵*/
    uint16_t red_1_robot_HP;
    uint16_t red_2_robot_HP;
    uint16_t red_3_robot_HP;
    uint16_t red_4_robot_HP;
    uint16_t red_5_robot_HP;
    uint16_t red_7_robot_HP;
    /*red_outpost_HP:
     *红方前哨站血量*/
    uint16_t red_outpost_HP;
    /*red_base_HP:
     *红方基地血量*/
    uint16_t red_base_HP;

    /*blue_x_robot_HP:
     *蓝方x号机器人血量
     *1:英雄
     *2:工程
     *3:3号步兵
     *4:4号步兵
     *5:5号步兵
     *7:哨兵*/
    uint16_t blue_1_robot_HP;
    uint16_t blue_2_robot_HP;
    uint16_t blue_3_robot_HP;
    uint16_t blue_4_robot_HP;
    uint16_t blue_5_robot_HP;
    uint16_t blue_7_robot_HP;
    /*blue_outpost_HP:
     *蓝方前哨站血量*/
    uint16_t blue_outpost_HP;
    /*blue_base_HP:
     *蓝方基地血量*/
    uint16_t blue_base_HP;
}__attribute__((packed)) ext_game_robot_HP_t;
typedef struct //0101
{/*event_type:
    bit 0-2：
        bit 0：己方补给站 1 号补血点占领状态 1 为已占领；
        bit 1：己方补给站 2 号补血点占领状态 1 为已占领；
        bit 2：己方补给站 3 号补血点占领状态 1 为已占领；
    bit 3-5：己方能量机关状态：
        bit 3 为打击点占领状态，1 为占领；
        bit 4 为小能量机关激活状态，1 为已激活；
        bit 5 为大能量机关激活状态，1 为已激活；
    bit 6：己方侧 R2/B2 环形高地占领状态 1 为已占领；
    bit 7：己方侧 R3/B3 梯形高地占领状态 1 为已占领；
    bit 8：己方侧 R4/B4 梯形高地占领状态 1 为已占领；
    bit 9：己方基地护盾状态：
        1 为基地有虚拟护盾血量；
        0 为基地无虚拟护盾血量；
    bit 10：己方前哨战状态：
        1 为前哨战存活；
        0 为前哨战被击毁；
    bit 10 -31: 保留*/
    unsigned int event_type;
}__attribute__((packed)) ext_event_data_t;

typedef struct //0x0102
{
    /*supply_projectile_id:
     *补给站口 ID：
         *1:1号补给口
         *2:2号补给口
     */
    uint8_t supply_projectile_id;
    /*supply_robot_id:
    *补弹机器人ID：
         * 0 为当前无机器人补弹
         * 1 为红方英雄机器人补弹
         * 2 为红方工程机器人补弹
         * 3/4/5 为红方步兵机器人补弹
         * 101 为蓝方英雄机器人补弹
         * 102 为蓝方工程机器人补弹
         * 103/104/105 为蓝方步兵机器人补弹
  */
    uint8_t supply_robot_id;
    /*supply_projectile_step:
     * 出弹口开闭状态：
         * 0 为关闭，1 为子弹准备中，2 为子弹下落
     */
    uint8_t supply_projectile_step;
    /*supply_projectile_num:
     *补弹数量：
         * 50：50 颗子弹
         * 100：100 颗子弹
         * 150：150 颗子弹
         * 200：200 颗子弹
     */
    uint8_t supply_projectile_num;
}__attribute__((packed)) ext_supply_projectile_action_t;

typedef struct //0x0103
{
    uint8_t supply_projectile_id;
    uint8_t supply_robot_id;
    uint8_t supply_num;
}__attribute__((packed)) ext_supply_projectile_booking_t;

typedef struct {
    /*level:
     *警告等级：
         * 1：黄牌
         * 2：红牌
         * 3：判负
     */
    uint8_t level;
    /*foul_robot_id:
     * 犯规机器人 ID：
         * 判负时，机器人 ID 为 0
         * 黄牌、红牌时，机器人 ID 为犯规机器人 ID
     */
    uint8_t foul_robot_id;
}__attribute__((packed)) ext_referee_warning_t;

typedef struct {
    /*dart_remaining_time:
     飞镖发射口倒计时:
        *15s 倒计时*/
    uint8_t dart_remaining_time;
}__attribute__((packed)) ext_dart_remaining_time_t;

typedef struct //0x0201
{
    /*robot_id:
     *本机器人 ID：
         *1：红方英雄机器人
         *2：红方工程机器人
         *3/4/5：红方步兵机器人
         *6：红方空中机器人
         *7：红方哨兵机器人
         *8：红方飞镖机器人
         *9：红方雷达站
         *101：蓝方英雄机器人
         *102：蓝方工程机器人
         *103/104/105：蓝方步兵机器人
         *106：蓝方空中机器人
         *107：蓝方哨兵机器人
         *108：蓝方飞镖机器人
         *109：蓝方雷达站。
    */
    uint8_t robot_id;
    /*robot_level:
     *机器人等级：
         *1：一级
         *2：二级
         *3：三级
     */
    uint8_t robot_level;
    /*remain_HP:
     *机器人剩余血量*/
    uint16_t remain_HP;
    /*max_HP:
     * 机器人上限血量*/
    uint16_t max_HP;

    /*shooter_id1_17mm_cooling_rate:
     * 机器人 1 号 17mm 枪口每秒冷却值*/
    uint16_t shooter_id1_17mm_cooling_rate;
    /*shooter_id1_17mm_cooling_limit:
     * 机器人 1 号 17mm 枪口热量上限*/
    uint16_t shooter_id1_17mm_cooling_limit;
    /*shooter_id1_17mm_speed_limit:
     * 2 机器人 1 号 17mm 枪口上限速度 单位 m/s*/
    uint16_t shooter_id1_17mm_speed_limit;

    /*shooter_id2_17mm_cooling_rate:
     * 机器人 2 号 17mm 枪口每秒冷却值*/
    uint16_t shooter_id2_17mm_cooling_rate;
    /*shooter_id2_17mm_cooling_limit:
     * 机器人 2 号 17mm 枪口热量上限*/
    uint16_t shooter_id2_17mm_cooling_limit;
    /*shooter_id2_17mm_speed_limit:
     * 机器人 2 号 17mm 枪口上限速度 单位 m/s*/
    uint16_t shooter_id2_17mm_speed_limit;

    /*shooter_id1_42mm_cooling_rate:
     * 机器人 42mm 枪口每秒冷却值*/
    uint16_t shooter_id1_42mm_cooling_rate;
    /*shooter_id1_42mm_cooling_limit:
     * 机器人 42mm 枪口热量上限*/
    uint16_t shooter_id1_42mm_cooling_limit;
    /*shooter_id1_42mm_speed_limit:
     * 机器人 42mm 枪口上限速度 单位 m/s*/
    uint16_t shooter_id1_42mm_speed_limit;

    /*chassis_power_limit
     * 机器人底盘功率限制上限*/
    uint16_t chassis_power_limit;
    /*主控电源输出情况：
        0 bit：gimbal 口输出： 1 为有 24V 输出，0 为无 24v 输出
        1 bit：chassis 口输出：1 为有 24V 输出，0 为无 24v 输出
        2 bit：shooter 口输出：1 为有 24V 输出，0 为无 24v 输出
     */
    uint8_t mains_power_gimbal_output: 1;
    uint8_t mains_power_chassis_output: 1;
    uint8_t mains_power_shooter_output: 1;
}__attribute__((packed)) ext_game_robot_state_t;

typedef struct //0x0202
{
    /*chassis_volt:
     * 底盘输出电压 单位 毫伏*/
    uint16_t chassis_volt;
    /*chassis_current:
     * 底盘输出电流 单位 毫安*/
    uint16_t chassis_current;
    /*chassis_power:
     * 底盘输出功率 单位 W 瓦*/
    float chassis_power;
    /*chassis_power_buffer:
     * 底盘功率缓冲 单位 J 焦耳 备注：飞坡根据规则增加至 250J*/
    uint16_t chassis_power_buffer;
    /*shooter_id1_17mm_cooling_heat
     * 1 号 17mm 枪口热量*/
    uint16_t shooter_id1_17mm_cooling_heat;
    /*shooter_id2_17mm_cooling_heat
     *2 号 17mm 枪口热量*/
    uint16_t shooter_id2_17mm_cooling_heat;
    /*shooter_id1_42mm_cooling_heat
     * 42mm 枪口热量*/
    uint16_t shooter_id1_42mm_cooling_heat;
}__attribute__((packed)) ext_power_heat_data_t;

typedef struct //0x0203
{
    /*x:
     * 位置 x 坐标，单位 m*/
    float x;
    /*y:
     * 位置 y 坐标，单位 m*/
    float y;
    /*z:
     * 位置 z 坐标，单位 m*/
    float z;
    /*yaw
     * 位置枪口，单位度*/
    float yaw;
}__attribute__((packed)) ext_game_robot_pos_t;

typedef struct //0x0204
{   /*power_rune_buff:
     *机器人增益：
         * bit 0：机器人血量补血状态
         * bit 1：枪口热量冷却加速
         * bit 2：机器人防御加成
         * bit 3：机器人攻击加成
         * 其他 bit 保留
     */
    uint8_t power_rune_buff;
}__attribute__((packed)) ext_buff_musk_t;

typedef struct //0x0205
{
    /*attack_time:
     * 可攻击时间 单位 s。30s 递减至 0*/
    uint8_t attack_time;
}__attribute__((packed)) aerial_robot_energy_t;

typedef struct //0x0206
{
    /*armor_type:
        * bit 0-3：当血量变化类型为装甲伤害，代表装甲 ID，其中数值为 0-4 号代表机器人的五个装甲片，其他血量变化类型，该变量数值为 0。
     * hurt_type:
        * bit 4-7：血量变化类型
            * 0x0 装甲伤害扣血
            * 0x1 模块掉线扣血
            * 0x2 超射速扣血
            * 0x3 超枪口热量扣血
            * 0x4 超底盘功率扣血
            * 0x5 装甲撞击扣血
    */
    uint8_t armor_type: 4;
    uint8_t hurt_type: 4;
}__attribute__((packed)) ext_robot_hurt_t;

typedef struct //0x0207
{
    /*bullet_type:
     *子弹类型:
        *1：17mm 弹丸 2：42mm 弹丸
     */
    uint8_t bullet_type;
    /*shooter_id:
     *发射机构 ID：
         * 1：1 号 17mm 发射机构
         * 2：2 号 17mm 发射机构
         * 3：42mm 发射机构
     */
    uint8_t shooter_id;
    /*bullet_freq:
     *子弹射频 单位 Hz*/
    uint8_t bullet_freq;
    /*bullet_speed:
     *子弹射速 单位 m/s*/
    float bullet_speed;
}__attribute__((packed)) ext_shoot_data_t;
typedef struct //0x0208
{
    /*bullet_remaining_num_17mm
     *17mm 子弹剩余发射数目*/
    uint16_t bullet_remaining_num_17mm;
    /*bullet_remaining_num_42mm
     *42mm 子弹剩余发射数目*/
    uint16_t bullet_remaining_num_42mm;
    /*coin_remaining_num
     *剩余金币数量*/
    uint16_t coin_remaining_num;
}__attribute__((packed)) ext_bullet_remaining_t;
typedef struct //0x0209
{
    /*rfid_status:
     *机器人 RFID 状态:
         * bit 0：基地增益点 RFID 状态
         * bit 1：高地增益点 RFID 状态
         * bit 2：能量机关激活点 RFID 状态
         * bit 3：飞坡增益点 RFID 状态
         * bit 4：前哨岗增益点 RFID 状态
         * bit 6：补血点增益点 RFID 状态
         * bit 7：工程机器人复活卡 RFID 状态
         * bit 8-31：保留
     * */
    uint32_t rfid_status;
}__attribute__((packed)) ext_rfid_status_t;
typedef struct //0x020A
{
    /*dart_launch_opening_status
     * 当前飞镖发射口的状态:
         * 1：关闭
         * 2：正在开启或者关闭中
         * 0：已经开启
     * */
    uint8_t dart_launch_opening_status;
    /*dart_attack_target:
     * 飞镖的打击目标，默认为前哨站
         * 0：前哨站
         * 1：基地
     * */
    uint8_t dart_attack_target;
    /*target_change_time:
     * 切换打击目标时的比赛剩余时间，单位秒，从未切换默认为0*/
    uint16_t target_change_time;
    /*operate_launch_cmd_time:
     * 最近一次操作手确定发射指令时的比赛剩余时间，单位秒, 初始值为 0*/
    uint16_t operate_launch_cmd_time;
}__attribute__((packed)) ext_dart_client_cmd_t;
typedef struct //0x0301
{
    uint16_t send_ID;
    uint16_t receiver_ID;
    uint16_t data_cmd_id;
    uint16_t data_len;
    uint8_t *data;
}__attribute__((packed)) ext_student_interactive_data_t;

typedef struct {
    float data1;
    float data2;
    float data3;
    uint8_t data4;
}__attribute__((packed)) custom_data_t;

typedef struct {
    uint8_t data[64];
}__attribute__((packed)) ext_up_stream_data_t;

typedef struct {
    uint8_t data[32];
}__attribute__((packed)) ext_download_stream_data_t;

/**
  * @brief          裁判系统任务
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
extern void referee_usart_task(void const *argument);

void init_referee_struct_data(void);

void referee_unpack_fifo_data(void);

void referee_data_solve(uint8_t *frame);

#endif //CPROJECT_REFEREE_DATA_H
