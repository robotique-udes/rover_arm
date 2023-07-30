#pragma once

#include <Arduino.h>
#include "ros.h"
#include "PIDRover/PIDRover.hpp"
#include "rover_arm_msgs/joint_state.h"
#include "DCMotor/DCMotor.hpp"

#define STOPPED 0
#define RUNNING 1

namespace rover_arm_lib
{
    class JointController
    {
    private:
        ros::NodeHandle *n = NULL;
        DCMotor* dc_motor = NULL;

        PIDRover pid;
        ros::Publisher pub_joint_state;
        rover_arm_msgs::joint_state joint_state_msg;

        bool pid_state = STOPPED;

        float target_speed;

    public:
        JointController(ros::NodeHandle* p_nodehandle,
                        double kp, double ki, double kd,
                        const char* name_pub_joint_state,
                        DCMotor* p_dc_motor);

        void setPIDLimits(double min, double max) {pid.SetOutputLimits(min, max);}
        //In milliseconds
        void setPIDRate(int rate){pid.SetSampleTime(rate);}
        void startPID(){pid.SetMode(AUTOMATIC); pid_state = RUNNING;}
        void stopPID(){pid.SetMode(MANUAL); pid_state = STOPPED;}
    };
}
