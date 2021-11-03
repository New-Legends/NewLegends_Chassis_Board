//
// Created by WSJ on 2021/11/2.
//

#ifndef CLASSIS_BOARD_PID_H
#define CLASSIS_BOARD_PID_H

#ifdef __cplusplus

#include "struct_typedef.h"
enum PID_MODE {
    PID_POSITION = 0,
    PID_DELTA
};

class pid {
public:
    uint8_t mode;
    //PID 三参数
    fp32 Kp;
    fp32 Ki;
    fp32 Kd;

    fp32 max_out;  //最大输出
    fp32 max_iout; //最大积分输出

    fp32 set;
    fp32 fdb;

    fp32 out;
    fp32 Pout;
    fp32 Iout;
    fp32 Dout;
    fp32 Dbuf[3];  //微分项 0最新 1上一次 2上上次
    fp32 error[3]; //误差项 0最新 1上一次 2上上次

    void PID_init(uint8_t mode, const fp32 *PID, fp32 max_out, fp32 max_iout);
    fp32 PID_calc( fp32 ref, fp32 set);
};

#endif
#endif //CLASSIS_BOARD_PID_H
