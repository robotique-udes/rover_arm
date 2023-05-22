#include "DCMotor.hpp"

rover_arm_lib::DCMotor::DCMotor(ros::NodeHandle *node_handle_ptr, short PWM,
                                const short DIR, const short out,
                                const short dir, const short I2C)
    : encoder(node_handle_ptr, out, dir, I2C)
{
    PWM_PIN = PWM;
    analogWrite(PWM_PIN, 0);

    DIR_PIN = DIR;
    pinMode(DIR_PIN, OUTPUT);
}

void rover_arm_lib::DCMotor::setDirection(bool direction)
{
    curr_direction = direction;
    digitalWrite(DIR_PIN, curr_direction);
}

// PWM can range from [0; 255] Direction control has to be done with setDirection method
void rover_arm_lib::DCMotor::setPWM(short pwm_value)
{
    analogWrite(PWM_PIN, pwm_value);
}

float rover_arm_lib::DCMotor::updateLoop()
{
    current_speed = static_cast<float>(encoder.updateLoop());
    return current_speed;
}
