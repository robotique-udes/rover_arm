#include <Arduino.h>
#include "TimerOne.h"
#include "ros.h"
#include "StepperMotor/StepperMotor.hpp"
#include "PID_v1.h"

#define JOINT_NB "j1" // As a string for param
#define PIN_PWM 6
#define PIN_ENABLE 4
#define PIN_DIR 13
#define BAUD 57600

#define PARAM_STRING_HEAD_SIZE 3
#define PARAM_MAX_STRING_LENGTH 50
const char *PARAM_STRINGS[PARAM_STRING_HEAD_SIZE] = {"/joints/",
                                                     JOINT_NB,
                                                     "/"};
char PARAM_HEAD_STRING[PARAM_MAX_STRING_LENGTH] = {'\0'};

ros::NodeHandle n;

void rosInit();
int strLength(const char *head);
void isr_stepper();
int getRosIntParam(const char *param_name);
rover_arm_lib::StepperMotor *p_stepper = NULL;

void setup()
{
    rosInit();
    delay(2000);

    rover_arm_lib::StepperMotor stepper(&n,
                                        getRosIntParam("PWM"),
                                        getRosIntParam("DIR"),
                                        getRosIntParam("EN"),
                                        getRosIntParam("gearbox"),
                                        getRosIntParam("step_per_turn"),
                                        getRosIntParam("ENCODER"),
                                        7,
                                        getRosIntParam("open_loop"));
    p_stepper = &stepper;
    stepper.begin(isr_stepper, DISABLE, 25.0f);

    while (1)
    {
        n.spinOnce();

    }
}

void rosInit()
{
    n.initNode();
    n.negotiateTopics();

    // Preallocation for performance purposes
    for (short i = 0; i < PARAM_STRING_HEAD_SIZE; i++)
    {
        strncat(PARAM_HEAD_STRING, PARAM_STRINGS[i], strLength(PARAM_STRINGS[i]));
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

// Interface for simplicity
int getRosIntParam(const char *param_name)
{
    int param_var = 0;
    char buffer[PARAM_MAX_STRING_LENGTH] = {"\0"};
    strncat(buffer, PARAM_HEAD_STRING, strLength(PARAM_HEAD_STRING));
    strncat(buffer, param_name, strLength(param_name));
    n.getParam(buffer, &param_var, 1);

    return param_var;
}

void isr_stepper()
{
    if (p_stepper->getEnable() == ENABLE)
    {
        p_stepper->step_counter += p_stepper->getDirection() == DIR_POSITIVE ? 1 : -1;
        digitalWrite(p_stepper->getPWMPin(), HIGH);
        digitalWrite(p_stepper->getPWMPin(), LOW);
    }
}
