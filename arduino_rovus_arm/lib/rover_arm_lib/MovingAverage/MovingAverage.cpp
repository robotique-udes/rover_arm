#include "MovingAverage.hpp"

rover_arm_lib::MovingAverage::MovingAverage() 
{
}

void rover_arm_lib::MovingAverage::addValue(double value)
{
    avg -= values[cursor];
    values[cursor] = value;
    avg += values[cursor];
    cursor = (cursor + 1) == AVG_SIZE ? 0 : (cursor + 1);
}
