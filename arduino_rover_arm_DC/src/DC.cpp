#include <Arduino.h>
#include "ros.h"
#include "DCMotor/DCMotor.hpp"
#include "PIDRover/PIDRover.hpp"
#include "rover_arm_msgs/desired_joint_state.h"
#include "rover_arm_msgs/joint_state.h"
#include "rover_arm_msgs/joint_calib.h"

#define BAUD 57600
#define DRIVE_DIR_PIN 13
#define DRIVE_PWM_PIN 6
#define ENCODER_OUT_PIN A0
#define ENCODER_DIR_PIN 7
#define KP 200.0f
#define KI 200.0f
#define KD 0.0f
#define MAX_SPEED 17.0f // deg/s
#define PUB_RATE 25     // in milliseconds
#define CALIB_TIMEOUT 5 // in seconds

// Function prototype
void rosInit();
void pid_init();
void pubJointState();
void callbackSubDesiredJointState(const rover_arm_msgs::joint_state &msg);
// void callbackSrvCalibration(const rover_arm_msgs::joint_calibRequest &request,
//                             rover_arm_msgs::joint_calibResponse &responce);


ros::NodeHandle n;

rover_arm_lib::PIDRover pid(KP, KI, KD);
rover_arm_lib::DCMotor *dc_motor_ptr = NULL;

rover_arm_msgs::joint_state joint_state_msg;
ros::Publisher pub_joint_state("JS", &joint_state_msg);
ros::Subscriber<rover_arm_msgs::joint_state> sub_desired_joint_state("DJS",
                                                                            callbackSubDesiredJointState,
                                                                            1);

// ros::ServiceServer<rover_arm_msgs::joint_calibRequest,
//                    rover_arm_msgs::joint_calibResponse>
//     srv_calibration("calibration", callbackSrvCalibration);

bool PID_bypass = 0;

void setup()
{
    rover_arm_lib::DCMotor dc_motor(&n, DRIVE_PWM_PIN, DRIVE_DIR_PIN, ENCODER_OUT_PIN, ENCODER_DIR_PIN);
    dc_motor_ptr = &dc_motor;
    rosInit();
    pid_init();

    n.loginfo("ros initated");
    delay(1000);

    unsigned long prev_pub_time = millis();
    while (true)
    {
        n.spinOnce();

        if (PID_bypass)
        {
            dc_motor.updateLoop();
        }
        else
        {
            pid.setCurrentSpeed(dc_motor.updateLoop());
            dc_motor.setPWM(round(pid.getCommand()));
            pid.Compute();
        }

        if (prev_pub_time + PUB_RATE < millis())
        {
            pubJointState();
            prev_pub_time = millis();
        }
    }
}

void rosInit()
{
    n.getHardware()->setBaud(BAUD);
    n.initNode();
    n.subscribe(sub_desired_joint_state);
    n.advertise(pub_joint_state);
    // n.advertiseService<rover_arm_msgs::joint_calibRequest,
    //                    rover_arm_msgs::joint_calibResponse>(srv_calibration);
    n.negotiateTopics();
    n.spinOnce();
}

void pid_init()
{
    pid.SetOutputLimits(0.0f, 255.0f);
    pid.SetSampleTime(5);
    pid.SetMode(AUTOMATIC);
}

void callbackSubDesiredJointState(const rover_arm_msgs::joint_state &msg)
{
    if (dc_motor_ptr == NULL)
    {
        n.logerror("dc_motor not initialised");
        return;
    }

    PID_bypass = 0;
    if (abs(msg.speed) < 0.05f)
    {
        PID_bypass = 1;
        dc_motor_ptr->setPWM(0);
        pid.setTargetSpeed(0.0f);
        return;
    }
    else if (msg.speed > 0.0f)
    {
        dc_motor_ptr->setDirection(DIRECTION_POSITIVE);
    }
    else if (msg.speed < 0.0f)
    {
        dc_motor_ptr->setDirection(DIRECTION_NEGATIVE);
    }
    pid.setTargetSpeed(static_cast<double>(abs(msg.speed)));
}

void pubJointState()
{
    rover_arm_msgs::joint_state msg;
    if (dc_motor_ptr != NULL)
    {
        msg.angle = dc_motor_ptr->getAngle();
        msg.speed = dc_motor_ptr->getSpeed();
    }
    else
    {
        msg.angle = -100.0f;
        msg.speed = -100.0f;
        n.logerror("dc_motor_ptr == NULL");
    }

    pub_joint_state.publish(&msg);
}

void callbackSrvCalibration(const rover_arm_msgs::joint_calibRequest &request,
                            rover_arm_msgs::joint_calibResponse &responce)
{
    // responce.result = false;

    // // while (/*microswitch not clicked*/ 1)
    // // {

    // // }
}
