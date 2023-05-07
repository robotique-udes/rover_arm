#include "PIDRover.hpp"

rover_arm_lib::PIDRover::PIDRover(float kp, float ki, float kd):
    PID(&abs_current_speed, &command, &target_speed, kp, ki, kd, P_ON_E, DIRECT){}
