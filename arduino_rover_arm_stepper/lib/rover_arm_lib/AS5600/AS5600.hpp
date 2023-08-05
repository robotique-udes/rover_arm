#pragma once
#include <Arduino.h>
#include "MovingAverage/MovingAverage.hpp"
#include "ros.h"
#include "rover_arm_msgs/encoder_status.h"
#include <Wire.h>

// #define REVERSE_DIRECTION

#define RELATIVE 0
#define ABSOLUTE 1
#define GEARBOX_RATIO 1316.4f
#define AS5600_MAX_VALUE 1024
#define SPEED_CALC_PERIOD 0
#define ENCODER_STATE_RATE 2500
#define AVG_SIZE 10
#define SPEED_CORRECTION_FACTOR 0.795f
const float ANGLE_CONVERSION = 360.0f / (AS5600_MAX_VALUE * GEARBOX_RATIO);
const double RAW_2_REAL_SPEED = 1000000.0f * SPEED_CORRECTION_FACTOR * ANGLE_CONVERSION;

#if defined(REVERSE_DIRECTION)
#define DIRECTION_POSITIVE 0
#define DIRECTION_NEGATIVE 1
#else
#define DIRECTION_POSITIVE 1
#define DIRECTION_NEGATIVE 0
#endif // REVERSE_DIRECTION

//ENUM for ros encoder_state_msg
enum EncoderState{ SIGNAL_TOO_LOW = -1, SIGNAL_OK, SIGNAL_TOO_HIGH};

namespace rover_arm_lib
{
    class AS5600
    {
    private:
        //ROS
        ros::NodeHandle *n;

        // Angle
        float zero_calib = 0.0f;
        float zero_shift = 0.0f;
        short reading = 0;
        short curr_cadrant;
        short prev_cadrant = 0;
        short turn_count = 0;

        // Speed
        MovingAverage moving_average;
        unsigned long prev_micros;
        long prev_raw_angle = 0; // In encoder step
        long curr_raw_angle = 0; // In encoder step
        double curr_raw_speed = 0.0f;
        double curr_speed = 0.0f;
        double filtered_speed = 0.0f;

        // PINs
        short pin_out;
        short pin_dir;
        short pin_I2C;

        void updateReading();
        void updateCadrant();
        void updateTurnCount();
        void updateRawTotalAngle();
        double updateSpeed();
        void sendEncoderStatus();

    public:
        AS5600(ros::NodeHandle *node_handle_ptr, short out, short dir, short I2C);

        double updateLoop();
        void setZero(float shift = 0.0f);
        short getReading() { return reading; }
        double getAngle(bool type = RELATIVE);
        long getRawAngle() { return curr_raw_angle; }
        double getRawSpeed() { return curr_raw_speed; }
        double getSpeed() { return filtered_speed; }
    };
}
