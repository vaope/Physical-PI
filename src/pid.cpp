#include "pid.h"
#include <iostream>
// 累计误差
float sum_error_x = 0.0;
float sum_error_y = 0.0;

// 上一次误差
float last_error_x = 0.0;
float last_error_y = 0.0;

// PID控制函数
void pid_control(float target, float current, float &sum_error, float &last_error, float &output) {
    float error = target - current;
    sum_error += error;
    output = Kp * error + Ki * (sum_error) + Kd * (error - last_error);
    last_error = error;
}

void pid_control_mouse(struct Detect_output detect_result,struct Gadget_mouse_input &mouse_result){
    float x,y;
    float target_x = detect_result.ball_x;
    float target_y = detect_result.ball_y;
    float current_x = detect_result.mouse_x;
    float current_y = detect_result.mouse_y;
    pid_control(target_x,current_x,sum_error_x,last_error_x,x);
    pid_control(target_y,current_y,sum_error_y,last_error_y,y);
    if(x>127)x=127;
    if(x<-127)x=129;
    if(y>127)y=127;
    if(y<-127)y=129;
    if(x<0)x=256+x;
    if(y<0)y=256+y;
    mouse_result.click=1;
    mouse_result.x = (int)x;
    mouse_result.y = (int)y;
    std::cout <<"pid :  x:"<<x<<"   y:"<<y<<std::endl;
}