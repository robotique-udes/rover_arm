#pragma once
#include <Arduino.h>

#define AVG_SIZE 10

namespace rover_arm_lib
{
    class MovingAverage
    {
    private:
        short cursor = 0;
        double values[AVG_SIZE] = {0};
        double avg = 0.0f;

    public:
        MovingAverage();
        void addValue(double value);
        double getAverage() { return (avg / AVG_SIZE); };
    };
}
