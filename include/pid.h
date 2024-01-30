#ifndef PID_H
#define PID_H

#include "v5lite.h"
#include "gadget_mouse.h"

// PID参数
#define Kp 0.5  // 比例增益
#define Ki 0.01 // 积分增益
#define Kd 0.02 // 微分增益

void pid_control_mouse(struct Detect_output detect_result,struct Gadget_mouse_input &mouse_result);

#endif //!PID_H