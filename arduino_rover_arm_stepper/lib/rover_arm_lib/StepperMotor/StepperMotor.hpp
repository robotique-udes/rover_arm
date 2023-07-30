#pragma once

#include <Arduino.h>
#include "EMotor/EMotor.hpp"
#include "TimerOne.h"
#include "limits.h"

#define DISABLE 1
#define ENABLE 0
#define DIR_POSITIVE 1
#define DIR_NEGATIVE 0

namespace rover_arm_lib
{
    class StepperMotor : public EMotor
    {
    private:
        TimerOne timer;
        short PIN_ENABLE;
        float speed_2_freq;
        float step_per_deg;
        bool enable_state;
        unsigned long step_period_micros;
        unsigned long prev_step_micros;

    public:
        long step_counter = 0;

        StepperMotor(ros::NodeHandle *node_handle_ptr,
                     const short PULSE,
                     const short DIR,
                     const short enable,
                     const int gearbox,
                     const unsigned long step_per_turn,
                     bool open_loop = true);

        void begin(void (*isr)(), bool enable, float speed = 0.0f);

        void setEnable(bool state);
        bool getEnable() { return enable_state; }
        void setSpeed(float speed);
        float virtual getSpeed() { return current_speed; }
        float virtual getAngle() { return static_cast<float>(step_counter) / step_per_deg; }
        void virtual setZero(float shift = 0.0f);
    };
}
