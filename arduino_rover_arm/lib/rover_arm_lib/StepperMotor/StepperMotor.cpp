#include "StepperMotor.hpp"

using namespace rover_arm_lib;

StepperMotor::StepperMotor(ros::NodeHandle *node_handle_ptr,
                                          const short PULSE,
                                          const short DIR,
                                          const short enable,
                                          const int gearbox,
                                          const unsigned long step_per_turn,
                                          bool open_loop)
    : EMotor(PULSE, DIR)
{
    PIN_ENABLE = enable;
    pinMode(PIN_ENABLE, OUTPUT);
    setEnable(DISABLE);

    speed_2_freq = static_cast<float>(gearbox * step_per_turn) / (360.0f * 1000000.0f);
    step_per_deg = gearbox * step_per_turn / 360.0f;
}

void StepperMotor::begin(void (*isr)(), bool enable, float speed)
{
    timer.attachInterrupt(isr);
    timer.initialize();
    setEnable(enable);
    setSpeed(speed);
}

void StepperMotor::setEnable(bool state)
{
    digitalWrite(PIN_ENABLE, state);
    enable_state = state;
}

void StepperMotor::setSpeed(float speed)
{
    if (speed > 0.0f)
    {
        setDirection(DIR_POSITIVE);
    }
    else
    {
        setDirection(DIR_NEGATIVE);
    }
    current_speed = speed;
    
    speed = abs(speed);
    if (speed < 0.001)
    {
        timer.setPeriod(ULONG_MAX);
        return;
    }
    
    step_period_micros = static_cast<unsigned long>(1.0f / (speed * speed_2_freq));
    timer.setPeriod(step_period_micros);
}

void StepperMotor::setZero(float shift)
{
    step_counter = static_cast<unsigned long>(shift*step_per_deg); 
}
