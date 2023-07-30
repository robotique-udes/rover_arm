#include <Arduino.h>
#include "ros.h"
#include "StepperMotor/StepperMotor.hpp"
#include "PID_v1.h"
#include "rover_arm_msgs/joint_state.h"
#include "rover_arm_msgs/watchdog.h"
#include "rover_arm_msgs/joint_calib.h"
#include "std_msgs/Float32.h"

#define OPEN_LOOP 1
#define GEARBOX 77
#define STEP_PER_TURN 1600
#define PWM 6
#define DIR 13
#define EN 4
#define ENCODER 14

#define JOINT_NB "j0"
#define PARAM_HEAD_STRING "/arm/" JOINT_NB "/" // Set joint number here
#define BAUD 57600

#define PERIOD_JOINT_STATE 10
#define PERIOD_WATCHDOG 1000

int strLength(const char *head);
void isrStepper();
void cancelExecution();
void callbackWatchdog(const rover_arm_msgs::watchdog &data);
void callbackDesiredJointState(const rover_arm_msgs::joint_state &data);
void callbackCalibration(const std_msgs::Float32 &msg);

bool watchdog_connected = false;
rover_arm_lib::StepperMotor *p_stepper = NULL;

void setup()
{
    ros::NodeHandle n;
    n.getHardware()->setBaud(BAUD);
    n.initNode();

    rover_arm_msgs::joint_state pub_msg;
    ros::Publisher pub_joint_state(PARAM_HEAD_STRING "JS", &pub_msg);                            // joint_state
    ros::Subscriber<rover_arm_msgs::joint_state> sub_desired_joint_state(PARAM_HEAD_STRING "DJS", // desired_joint_state
                                                                        callbackDesiredJointState,
                                                                        1);
    ros::Subscriber<rover_arm_msgs::watchdog> sub_watchdog("/arm/JWD", callbackWatchdog, 1); // joint_watchdog
    ros::Subscriber<std_msgs::Float32> server_calib(PARAM_HEAD_STRING "C", callbackCalibration);

    n.advertise(pub_joint_state);
    n.subscribe(sub_watchdog);
    n.subscribe(sub_desired_joint_state);
    n.subscribe(server_calib);
    n.negotiateTopics();
    n.spinOnce();

    rover_arm_lib::StepperMotor stepper(&n, PWM, DIR, EN, GEARBOX, STEP_PER_TURN, OPEN_LOOP);
    p_stepper = &stepper;
    stepper.begin(isrStepper, ENABLE);

    delay(2500);
    n.loginfo("Joint is ready");

    // Rate variables
    unsigned long next_watchdog_time = millis();
    unsigned long next_joint_state_time = millis();

    while (1)
    {
        n.spinOnce();

        // Watchdog check loop
        if (next_watchdog_time < millis())
        {
            next_watchdog_time = millis() + PERIOD_WATCHDOG;

            if (!watchdog_connected)
            {
                stepper.setEnable(DISABLE);
            }
            else
            {
                stepper.setEnable(ENABLE);
            }
            watchdog_connected = false;
        }

        // joint state pub loop
        if (next_joint_state_time < millis())
        {
            next_joint_state_time = millis() + PERIOD_JOINT_STATE;

            rover_arm_msgs::joint_state msg;
            msg.speed = stepper.getSpeed();
            msg.angle = stepper.getAngle();
            pub_joint_state.publish(&msg);
        }
    }
}

// Max 100 characters
int strLength(const char *head)
{
    short counter = 0;
    while (*head++ != static_cast<unsigned short>(0) && counter < 100)
    {
        counter++;
    }

    return counter;
}

void isrStepper()
{
    p_stepper->step_counter += p_stepper->getDirection() == DIR_POSITIVE ? 1 : -1;
    digitalWrite(p_stepper->PIN_PULSE, HIGH);
    digitalWrite(p_stepper->PIN_PULSE, LOW);
}

void callbackWatchdog(const rover_arm_msgs::watchdog &data)
{
    watchdog_connected = true;
}

void callbackDesiredJointState(const rover_arm_msgs::joint_state &data)
{
    p_stepper->setSpeed(data.speed);
}

void callbackCalibration(const std_msgs::Float32 &msg)
{
    p_stepper->setZero(msg.data);
}
