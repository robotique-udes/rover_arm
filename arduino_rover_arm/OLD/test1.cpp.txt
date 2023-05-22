#include <Arduino.h>
#include "ros.h"
#include "rover_arm_msg/infos.h"
#include "dc_motor/dc_motor.hpp"
#include "PID_v1.h"

#define DIR_PIN 13
#define PWM_PIN 6
#define OUT_PIN A0
#define DIR_PIN_ENCODER 7
#define ADDRESS_ENCODEUR 0x36
#define PRINT_TIME 20

void sub_pid_coeff_callback(const rover_arm_msg::infos &msg);

ros::NodeHandle n;

unsigned long prev_millis = millis();

double target_speed = 0.0f;
double current_speed = 0.0f;
double abs_current_speed = 0.0f;
double command = 0.0f;

PID pid_speed(&abs_current_speed, &command, &target_speed, 200, 200, 0, P_ON_E, DIRECT);
rover_arm_lib::DCMotor *dc_motor_ptr = NULL;

void setup()
{
    rover_arm_lib::DCMotor dc_motor = rover_arm_lib::DCMotor(&n, PWM_PIN, DIR_PIN, OUT_PIN, DIR_PIN_ENCODER);
    dc_motor_ptr = &dc_motor;

    ros::Subscriber<rover_arm_msg::infos> sub_pid_coeff("pid_coeff", sub_pid_coeff_callback, 1);

    rover_arm_msg::infos info;
    ros::Publisher pub_info("joint_info", &info);

    n.getHardware()->setBaud(57600);
    n.initNode();
    n.subscribe(sub_pid_coeff);
    n.advertise(pub_info);
    n.negotiateTopics();

    pinMode(7, OUTPUT);
    digitalWrite(7, 0);

    dc_motor.setDirection(DIRECTION_POSITIVE);

    pid_speed.SetOutputLimits(0.0f, 255.0f);
    pid_speed.SetSampleTime(5);
    pid_speed.SetMode(AUTOMATIC);

    unsigned long prev_millis = millis();

    while (1)
    {
        while (!n.connected())
        {
            n.spinOnce();
            dc_motor.setPWM(0);
        }
        

        current_speed = -dc_motor.updateLoop();
        abs_current_speed = abs(current_speed);
        dc_motor.setPWM(command);
        pid_speed.Compute();

        if (prev_millis + PRINT_TIME < millis())
        {
            rover_arm_msg::infos msg;
            msg.curr_speed = static_cast<float>(current_speed);
            msg.pid_coeffs[0] = static_cast<float>(dc_motor.getAngle());
            msg.pid_coeffs[1] = 0.0;
            msg.pid_coeffs[2] = 0.0;
            msg.command = command;
            pub_info.publish(&msg);

            prev_millis = millis();
        }
        n.spinOnce();
    }
}

void sub_pid_coeff_callback(const rover_arm_msg::infos &msg)
{
    if (msg.command > 0.0f)
    {
        dc_motor_ptr->setDirection(DIRECTION_POSITIVE);
    }
    else
    {
        dc_motor_ptr->setDirection(DIRECTION_NEGATIVE);
    }

    if (msg.pid_coeffs[2] > 0.5f)
        dc_motor_ptr->setZero(0.0f);
    
    target_speed = double(abs(msg.command));

    n.logwarn("PID coeffs updated");
}
