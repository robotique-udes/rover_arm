#pragma once

#include <Arduino.h>
#include "DCMotor/DCMotor.hpp"
#include "TimerOne.h"
#include "limits.h"

#define DISABLE 1
#define ENABLE 0
#define DIR_POSITIVE 0
#define DIR_NEGATIVE 1

namespace rover_arm_lib
{
    class StepperMotor : public DCMotor
    {
    private:
        TimerOne timer;
        short ENABLE_PIN;
        float speed_2_freq;
        float step_per_deg;
        bool enable_state;

    public:
        long step_counter = 0;

        StepperMotor(ros::NodeHandle *node_handle_ptr,
                     const short PWM,
                     const short DIR,
                     const short enable,
                     const unsigned long gearbox,
                     const unsigned long step_per_turn,
                     const short out,
                     const short dir,
                     bool open_loop = false,
                     const short I2C = 0x36) : DCMotor(node_handle_ptr, PWM, DIR, out, dir, I2C)
        {
            ENABLE_PIN = enable;
            pinMode(ENABLE_PIN, OUTPUT);
            setEnable(DISABLE);

            speed_2_freq = static_cast<float>(gearbox * step_per_turn) / (360.0f * 1000000.0f);
            step_per_deg = gearbox * step_per_turn / 360.0f;
        }

        void begin(void (*isr)(), bool enable, float speed = -1.0f);
        void setEnable(bool state)
        {
            digitalWrite(ENABLE_PIN, state);
            enable_state = state;
        }
        bool getEnable() { return enable_state; }
        bool getDirection() { return curr_direction; }
        short getPWMPin() { return PWM_PIN; }
        float getAngle() { return static_cast<float>(step_counter) / step_per_deg; }
        void setSpeed(float speed);
        float getSpeed(){return current_speed;}
    };

    void StepperMotor::begin(void (*isr)(), bool enable, float speed)
    {

        timer.attachInterrupt(isr);
        timer.initialize();
        setEnable(enable);

        setSpeed(speed);
        // if (speed == 0.0f)
        // {
        //     timer.setPeriod(ULONG_MAX);
        // }

        // unsigned long period = static_cast<unsigned long>(1 / (speed * speed_2_freq));

        // timer.setPeriod(period);
    }

    void StepperMotor::setSpeed(float speed)
    {
        if (abs(speed) < 0.01)
        {
            timer.setPeriod(ULONG_MAX);
            return;
        } 
        current_speed = speed;
        timer.setPeriod(static_cast<unsigned long>(1 / (speed * speed_2_freq)));
    }

}
