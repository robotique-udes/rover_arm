#pragma once
#include <Arduino.h>
#include "AS5600/AS5600.hpp"

namespace rover_arm_lib
{
    class DCMotor
    {
    private:
        int DIR_PIN;
        int PWM_PIN;
        bool curr_direction;
        AS5600 encoder;

    public:
        DCMotor(ros::NodeHandle* node_handle_ptr, short PWM, const short DIR, const short out, const short dir, const short I2C = 0x36);
        void setDirection(bool direction);
        void setPWM(short pwm);
        float updateLoop(){ return static_cast<float>(encoder.updateLoop());}
        float getAngle() {return static_cast<float>(encoder.getAngle());}
        void setZero(float shift = 0.0f) {encoder.setZero(shift);};
        void setPublisher(ros::Publisher* pub_ptr) { encoder.setPublisher(pub_ptr);}

    };
}
