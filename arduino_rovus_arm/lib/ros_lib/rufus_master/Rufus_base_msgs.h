#ifndef _ROS_rufus_master_Rufus_base_msgs_h
#define _ROS_rufus_master_Rufus_base_msgs_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rufus_master
{

  class Rufus_base_msgs : public ros::Msg
  {
    public:
      typedef float _motor_FL_type;
      _motor_FL_type motor_FL;
      typedef float _motor_FR_type;
      _motor_FR_type motor_FR;
      typedef float _motor_BL_type;
      _motor_BL_type motor_BL;
      typedef float _motor_BR_type;
      _motor_BR_type motor_BR;

    Rufus_base_msgs():
      motor_FL(0),
      motor_FR(0),
      motor_BL(0),
      motor_BR(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_motor_FL;
      u_motor_FL.real = this->motor_FL;
      *(outbuffer + offset + 0) = (u_motor_FL.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_motor_FL.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_motor_FL.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_motor_FL.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->motor_FL);
      union {
        float real;
        uint32_t base;
      } u_motor_FR;
      u_motor_FR.real = this->motor_FR;
      *(outbuffer + offset + 0) = (u_motor_FR.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_motor_FR.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_motor_FR.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_motor_FR.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->motor_FR);
      union {
        float real;
        uint32_t base;
      } u_motor_BL;
      u_motor_BL.real = this->motor_BL;
      *(outbuffer + offset + 0) = (u_motor_BL.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_motor_BL.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_motor_BL.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_motor_BL.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->motor_BL);
      union {
        float real;
        uint32_t base;
      } u_motor_BR;
      u_motor_BR.real = this->motor_BR;
      *(outbuffer + offset + 0) = (u_motor_BR.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_motor_BR.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_motor_BR.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_motor_BR.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->motor_BR);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_motor_FL;
      u_motor_FL.base = 0;
      u_motor_FL.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_motor_FL.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_motor_FL.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_motor_FL.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->motor_FL = u_motor_FL.real;
      offset += sizeof(this->motor_FL);
      union {
        float real;
        uint32_t base;
      } u_motor_FR;
      u_motor_FR.base = 0;
      u_motor_FR.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_motor_FR.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_motor_FR.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_motor_FR.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->motor_FR = u_motor_FR.real;
      offset += sizeof(this->motor_FR);
      union {
        float real;
        uint32_t base;
      } u_motor_BL;
      u_motor_BL.base = 0;
      u_motor_BL.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_motor_BL.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_motor_BL.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_motor_BL.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->motor_BL = u_motor_BL.real;
      offset += sizeof(this->motor_BL);
      union {
        float real;
        uint32_t base;
      } u_motor_BR;
      u_motor_BR.base = 0;
      u_motor_BR.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_motor_BR.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_motor_BR.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_motor_BR.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->motor_BR = u_motor_BR.real;
      offset += sizeof(this->motor_BR);
     return offset;
    }

    virtual const char * getType() override { return "rufus_master/Rufus_base_msgs"; };
    virtual const char * getMD5() override { return "e621f9710f8a9ff8d191feb0dac8bcc4"; };

  };

}
#endif
