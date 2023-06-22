#ifndef _ROS_rover_arm_msgs_desired_joint_state_h
#define _ROS_rover_arm_msgs_desired_joint_state_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rover_arm_msgs
{

  class desired_joint_state : public ros::Msg
  {
    public:
      typedef int8_t _control_mode_type;
      _control_mode_type control_mode;
      typedef float _speed_type;
      _speed_type speed;
      typedef int16_t _pwm_type;
      _pwm_type pwm;
      enum { PWM_MODE =  1 };
      enum { PID_MODE =  2 };

    desired_joint_state():
      control_mode(0),
      speed(0),
      pwm(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_control_mode;
      u_control_mode.real = this->control_mode;
      *(outbuffer + offset + 0) = (u_control_mode.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->control_mode);
      union {
        float real;
        uint32_t base;
      } u_speed;
      u_speed.real = this->speed;
      *(outbuffer + offset + 0) = (u_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->speed);
      union {
        int16_t real;
        uint16_t base;
      } u_pwm;
      u_pwm.real = this->pwm;
      *(outbuffer + offset + 0) = (u_pwm.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_pwm.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->pwm);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_control_mode;
      u_control_mode.base = 0;
      u_control_mode.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->control_mode = u_control_mode.real;
      offset += sizeof(this->control_mode);
      union {
        float real;
        uint32_t base;
      } u_speed;
      u_speed.base = 0;
      u_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->speed = u_speed.real;
      offset += sizeof(this->speed);
      union {
        int16_t real;
        uint16_t base;
      } u_pwm;
      u_pwm.base = 0;
      u_pwm.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_pwm.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->pwm = u_pwm.real;
      offset += sizeof(this->pwm);
     return offset;
    }

    virtual const char * getType() override { return "rover_arm_msgs/desired_joint_state"; };
    virtual const char * getMD5() override { return "f2a08c93f4d2fdf8304c55c98012006e"; };

  };

}
#endif
