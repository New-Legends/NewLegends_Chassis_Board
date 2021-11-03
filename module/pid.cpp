//
// Created by WSJ on 2021/11/2.
//

#include "pid.h"

#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }

void pid::PID_init(uint8_t mode_, const fp32 PID[3], fp32 max_out_, fp32 max_iout_) {
    mode = mode_;
    Kp = PID[0];
    Ki = PID[1];
    Kd = PID[2];
    max_out = max_out_;
    max_iout = max_iout_;
    Dbuf[0] = Dbuf[1] = Dbuf[2] = 0.0f;
    error[0] = error[1] = error[2] = Pout = Iout = Dout = out = 0.0f;
}

fp32 pid::PID_calc(fp32 ref, fp32 set_) {
    error[2] = error[1];
    error[1] = error[0];
    set = set_;
    fdb = ref;
    error[0] = set - ref;
    if (mode == PID_POSITION) {
        Pout = Kp * error[0];
        Iout += Ki * error[0];
        Dbuf[2] = Dbuf[1];
        Dbuf[1] = Dbuf[0];
        Dbuf[0] = (error[0] - error[1]);
        Dout = Kd * Dbuf[0];
        LimitMax(Iout, max_iout);
        out = Pout + Iout + Dout;
        LimitMax(out, max_out);
    } else if (mode == PID_DELTA) {
        Pout = Kp * (error[0] - error[1]);
        Iout = Ki * error[0];
        Dbuf[2] = Dbuf[1];
        Dbuf[1] = Dbuf[0];
        Dbuf[0] = (error[0] - 2.0f * error[1] + error[2]);
        Dout = Kd * Dbuf[0];
        out += Pout + Iout + Dout;
        LimitMax(out, max_out);
    }
    return out;
}
