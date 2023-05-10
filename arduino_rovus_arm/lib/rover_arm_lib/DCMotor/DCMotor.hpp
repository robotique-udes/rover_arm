#pragma once
#include <Arduino.h>
#include "AS5600/AS5600.hpp"

namespace rover_arm_lib
{
    class DCMotor
    {
    protected:
        short PWM_PIN;
        int DIR_PIN;
        bool curr_direction;
        float current_speed;
        AS5600 encoder;

    public:


        DCMotor(ros::NodeHandle* node_handle_ptr, short PWM, const short DIR, const short out, const short dir, const short I2C = 0x36);
        void setDirection(bool direction);
        void setPWM(short pwm);
        float updateLoop();
        float getAngle() {return static_cast<float>(encoder.getAngle());}
        void setZero(float shift = 0.0f) {encoder.setZero(shift);};
        float getSpeed() {return -current_speed;}

    };
}
