#pragma once

#include <Arduino.h>
#include <PID_v1.h>

namespace rover_arm_lib
{
    class PIDRover : public PID
    {
        private:
            double target_speed = 0.0f;
            double abs_current_speed = 0.0f;
            double command = 0.0f;

        public:
            PIDRover(float Kp, float Ki, float Kd);
            void setTargetSpeed(float new_speed){target_speed = new_speed;}
            void setCurrentSpeed(float speed) {abs_current_speed = static_cast<double>(abs(speed));}
            float getCommand(){return static_cast<float>(command);}
    };
}
