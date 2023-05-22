#pragma once

#include <Arduino.h>
#include "AS5600/AS5600.hpp"

namespace rover_arm_lib
{
    // Abstract class for DCMotor and Steppers
    class EMotor
    {
    protected:
        bool curr_direction;
        float current_speed;
        void setDirection(bool direction)
        {
            digitalWrite(PIN_DIR, direction);
            curr_direction = direction;
        }

    public:
        short PIN_PULSE;
        int PIN_DIR;
        
        EMotor(const short pulse_pin, const short dir_pin)
        {
            PIN_PULSE = pulse_pin;
            pinMode(PIN_PULSE, OUTPUT);

            PIN_DIR = dir_pin;
            pinMode(PIN_DIR, OUTPUT);
        }
        bool getDirection() { return curr_direction; }
        void virtual setZero(float shift = 0.0f) = 0;
        float virtual getAngle() = 0;
        float virtual getSpeed() = 0; 
    };
}
