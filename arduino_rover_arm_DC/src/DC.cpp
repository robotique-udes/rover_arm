#include <Arduino.h>
#include "ros.h"
#include "DCMotor/DCMotor.hpp"
#include "PIDRover/PIDRover.hpp"
#include "rover_arm_msgs/joint_state.h"
#include "rover_arm_msgs/watchdog.h"
#include "rover_arm_msgs/PID.h"
#include "rover_arm_msgs/joint_calib.h"
#include "std_msgs/Float32.h"

#define BAUD 57600
#define DRIVE_DIR_PIN 13
#define DRIVE_PWM_PIN 6
#define ENCODER_OUT_PIN A0
#define ENCODER_DIR_PIN 7
#define KP 15.0f
#define KI 190.0f
#define KD 0.0f
#define MAX_SPEED 17.0f              // deg/s
#define PUB_RATE 25                  // in milliseconds
#define CALIB_TIMEOUT 5              // in seconds
#define PERIOD_WATCHDOG 1000         // in milliseconds
#define PERIOD_TROTTLED_TRACES 10000 // in milliseconds

// Function prototype
void rosInit();
void pid_init();
void pubJointState();
void callbackWatchdog(const rover_arm_msgs::watchdog &data);
void callbackSubDesiredJointState(const rover_arm_msgs::joint_state &msg);
void callbackSPIDValues(const rover_arm_msgs::PID &msg);
void callbackCalibration(const std_msgs::Float32 &msg);
// void callbackSrvCalibration(const rover_arm_msgs::joint_calibRequest &request,
//                             rover_arm_msgs::joint_calibResponse &responce);
void stop();
void checkWatchdog();

ros::NodeHandle n;

rover_arm_lib::PIDRover pid(KP, KI, KD);
rover_arm_lib::DCMotor *dc_motor_ptr = NULL;

rover_arm_msgs::joint_state joint_state_msg;
ros::Publisher pub_joint_state("JS", &joint_state_msg);
ros::Subscriber<rover_arm_msgs::watchdog> sub_watchdog("/arm/JWD", callbackWatchdog, 1); // joint_watchdog
ros::Subscriber<rover_arm_msgs::joint_state> sub_desired_joint_state("DJS",
                                                                     callbackSubDesiredJointState,
                                                                     1);
// ros::ServiceServer<rover_arm_msgs::joint_calibRequest,
//                    rover_arm_msgs::joint_calibResponse>
//     srv_calibration_server("C", callbackSrvCalibration);

ros::Subscriber<rover_arm_msgs::PID> sub_pid_values("PID", callbackSPIDValues, 1);
ros::Subscriber<std_msgs::Float32> sub_calibration("C", callbackCalibration, 1);

bool PID_bypass = 0;

bool watchdog_connected = false;
bool watchdog_OK = false;
unsigned long next_watchdog_time = millis();
unsigned long next_joint_state_time = millis();
unsigned long next_trace_trottle_time = millis();

void setup()
{
    rover_arm_lib::DCMotor dc_motor(&n, DRIVE_PWM_PIN, DRIVE_DIR_PIN, ENCODER_OUT_PIN, ENCODER_DIR_PIN);
    dc_motor_ptr = &dc_motor;
    rosInit();
    pid_init();

    // n.loginfo("ros initated");
    delay(1000);

    unsigned long prev_pub_time = millis();
    while (true)
    {
        n.spinOnce();
        checkWatchdog();

        if (watchdog_OK)
        {
            if (PID_bypass)
            {
                dc_motor.updateLoop();
            }
            else
            {
                pid.setCurrentSpeed(dc_motor.updateLoop());
                if (dc_motor_ptr->getDirection() == DIRECTION_NEGATIVE)
                {
                    dc_motor.setPWM(255 - round(pid.getCommand()));
                }
                else
                {
                    dc_motor.setPWM(round(pid.getCommand()));
                }

                pid.Compute();
            }
        }
        else
        {
            if (next_trace_trottle_time < millis())
            {
                next_trace_trottle_time = millis() + PERIOD_TROTTLED_TRACES;
                // n.logerror("Watchdog not connected");
            }
            stop();
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
    n.subscribe(sub_pid_values);
    n.subscribe(sub_watchdog);
    n.advertise(pub_joint_state);
    n.subscribe(sub_calibration);
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
        // n.logerror("dc_motor not initialised");
        return;
    }

    PID_bypass = 0;
    if (abs(msg.speed) < 0.05f)
    {
        PID_bypass = 1;
        stop();
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
        // n.logerror("dc_motor_ptr == NULL");
    }

    pub_joint_state.publish(&msg);
}

// Using topic to save very valuable SRAM
void callbackCalibration(const std_msgs::Float32 &msg)
{
    dc_motor_ptr->setZero(msg.data);
    return;
}

void callbackSPIDValues(const rover_arm_msgs::PID &msg)
{
    pid.SetTunings(msg.kP, msg.kI, msg.kD);
}

void stop()
{
    if (dc_motor_ptr->getDirection() == DIRECTION_NEGATIVE)
    {
        dc_motor_ptr->setPWM(255 - 0);
    }
    else
    {
        dc_motor_ptr->setPWM(0);
    }
}

void callbackWatchdog(const rover_arm_msgs::watchdog &data)
{
    watchdog_connected = true;
}

void checkWatchdog()
{
    if (next_watchdog_time < millis())
    {
        next_watchdog_time = millis() + PERIOD_WATCHDOG;

        if (!watchdog_connected)
        {
            watchdog_OK = false;
        }
        else
        {
            watchdog_OK = true;
        }
        watchdog_connected = false;
    }
}
