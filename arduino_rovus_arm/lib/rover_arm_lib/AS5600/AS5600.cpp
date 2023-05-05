#include "AS5600.hpp"

rover_arm_lib::AS5600::AS5600(ros::NodeHandle *node_handle_ptr, short out, short dir, short I2C, ros::Publisher *pub_ptr)
{
    n = node_handle_ptr;

    pin_out = out;
    pin_I2C = I2C;
    pin_dir = dir;

    prev_micros = micros();
    encoder_state_prev_millis = millis();

    pub_encoder_state = pub_ptr;
}

void rover_arm_lib::AS5600::updateReading()
{
    reading = analogRead(pin_out);
}

void rover_arm_lib::AS5600::updateCadrant()
{
    if (reading >= 0 && reading < 256)
    {
        curr_cadrant = 1;
    }
    else if (reading >= 256 && reading < 512)
    {
        curr_cadrant = 2;
    }
    else if (reading >= 512 && reading < 768)
    {
        curr_cadrant = 3;
    }
    else if (reading >= 768 && reading < 1024)
    {
        curr_cadrant = 4;
    }
}

void rover_arm_lib::AS5600::updateTurnCount()
{
    if (curr_cadrant == 1 && prev_cadrant == 4)
    {
        turn_count += 1;
        // Serial.println(turn_number);
    }
    else if (curr_cadrant == 4 && prev_cadrant == 1)
    {
        turn_count -= 1;
        // Serial.println(turn_number);
    }
    prev_cadrant = curr_cadrant;
}

void rover_arm_lib::AS5600::updateRawTotalAngle()
{
    if (turn_count >= 0)
        curr_raw_angle = reading + (AS5600_MAX_VALUE * long(turn_count));
    else if (turn_count < 0)
        curr_raw_angle = -(AS5600_MAX_VALUE - reading) + (AS5600_MAX_VALUE * long(turn_count));
}

double rover_arm_lib::AS5600::updateSpeed()
{
    curr_raw_speed = (curr_raw_angle - prev_raw_angle) / double((micros() - prev_micros));
    prev_micros = micros();
    prev_raw_angle = curr_raw_angle;

    curr_speed = RAW_2_REAL_SPEED * curr_raw_speed;

    if (abs(curr_speed) < 30.0f)
    {
        moving_average.addValue(curr_speed);
    }

    return abs(moving_average.getAverage()) > 0.1f ? moving_average.getAverage() : 0.0f;
}

double rover_arm_lib::AS5600::updateLoop()
{
    if (encoder_state_prev_millis + ENCODER_STATE_RATE < millis())
    {
        sendEncoderStatus();
        encoder_state_prev_millis = millis();
    }
    updateReading();
    updateCadrant();
    updateTurnCount();
    updateRawTotalAngle();
    return updateSpeed();
}

double rover_arm_lib::AS5600::getAngle(bool type)
{
    float angle;
    if (type == RELATIVE)
    {
        angle = (static_cast<float>(curr_raw_angle) * ANGLE_CONVERSION) - zero_calib + zero_shift;
    }
    else
    {
        angle = static_cast<float>(curr_raw_angle) * ANGLE_CONVERSION;
    }

// TODO Redo function with precisions and optimise memory
#if defined(REVERSE_DIRECTION)
    return -angle;
#else
    return angle;
#endif
}

void rover_arm_lib::AS5600::setZero(float shift)
{
#if defined(REVERSE_DIRECTION)
    zero_calib = -getAngle(ABSOLUTE) + shift;
#else
    zero_calib = getAngle(ABSOLUTE) - shift;
#endif
}

//TODO: Finish implementation
void rover_arm_lib::AS5600::sendEncoderStatus()
{
    if (pub_encoder_state == NULL)
    {
        return;
    }

    rover_arm_msg::encoder_status msg;
    Wire.begin();
    Wire.beginTransmission(0x36);
    // Getting status register
    Wire.write(0x0B);
    Wire.endTransmission(false);
    Wire.requestFrom(0x36, 1);
    Wire.read();

    Wire.endTransmission();
    Wire.end();

    msg.state = msg.SIGNAL_OK;
    pub_encoder_state->publish(&msg);
}
