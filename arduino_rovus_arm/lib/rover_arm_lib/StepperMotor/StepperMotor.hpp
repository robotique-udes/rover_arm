#pragma once

#include <Arduino.h>
#include "DCMotor/DCMotor.hpp"
#include "TimerOne.h"
#include "limits.h"

#define DISABLE 0
#define ENABLE 1

namespace rover_arm_lib
{
    class StepperMotor : public DCMotor
    {
    private:
        TimerOne timer;
        short ENABLE_PIN;
        float speed_2_period;

    public:
        StepperMotor(ros::NodeHandle *node_handle_ptr,
                     short PWM,
                     const short DIR,
                     const unsigned long gearbox,
                     const unsigned long step_per_turn,
                     const short out,
                     const short dir,
                     bool open_loop = false,
                     const short I2C = 0x36) : DCMotor(node_handle_ptr, PWM, DIR, out, dir, I2C)
        {
            pinMode(ENABLE_PIN, OUTPUT);
            digitalWrite(ENABLE_PIN, LOW);

            speed_2_period = static_cast<float>(gearbox*step_per_turn)/(360.0f*1000000.0f);
        }

        void begin(void (*isr)(), bool enable, float speed = -1.0f);

    };

    void StepperMotor::begin(void (*isr)(), bool enable, float speed)
    {
        timer.attachInterrupt(isr);
        timer.initialize();
        if(enable)
        {
            digitalWrite(ENABLE_PIN, HIGH);
        }

        if (speed == -1.0f)
        {
            timer.setPeriod(ULONG_MAX);
        }
        unsigned long period = static_cast<unsigned long>(1/(speed*speed_2_period));

        timer.setPeriod(period);
    }

}
