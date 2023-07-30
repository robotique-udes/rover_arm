#include "DCMotor.hpp"

rover_arm_lib::DCMotor::DCMotor(ros::NodeHandle *node_handle_ptr, short PWM,
                                const short DIR, const short out,
                                const short dir, const short I2C)
    : encoder(node_handle_ptr, out, dir, I2C)
{
    DIR_PIN = DIR;
    PWM_PIN = PWM;
    pinMode(DIR_PIN, OUTPUT);

     // Making sure motor is at full stop 
    digitalWrite(DIR_PIN, false);
    analogWrite(PWM_PIN, 0);
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
