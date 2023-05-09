#include <Arduino.h>
#include "TimerOne.h"
#include "ros.h"
#include "StepperMotor/StepperMotor.hpp"
#include "PID_v1.h"

#define JOINT_NB "1" // As a string for param
#define PIN_PWM 6
#define PIN_ENABLE 4
#define PIN_DIR 13
#define BAUD 57600

#define PARAM_STRING_HEAD_SIZE 3
#define PARAM_MAX_STRING_LENGTH 25
const char *PARAM_STRINGS[PARAM_STRING_HEAD_SIZE] = {"/joints/j",
                                                     JOINT_NB,
                                                     "/"};
char PARAM_HEAD_STRING[PARAM_MAX_STRING_LENGTH] = {'\0'};

ros::NodeHandle n;

void rosInit();
int strLength(const char *head);
void isr_stepper();
int getRosIntParam(const char *param_name);

void setup()
{
    rosInit();

    rover_arm_lib::StepperMotor stepper(&n, PIN_PWM, PIN_DIR, 100, 1600, A0, 7, true);
    stepper.begin(isr_stepper, DISABLE);

    while (1)
    {
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
    strncat(buffer, param_name, 8);
    n.getParam(buffer, &param_var, 1);

    return param_var;
}

void isr_stepper()
{
    digitalWrite(PIN_PWM, HIGH);
    digitalWrite(PIN_PWM, LOW);
}
