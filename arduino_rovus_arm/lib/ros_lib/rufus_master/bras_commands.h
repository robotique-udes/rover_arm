#ifndef _ROS_rufus_master_bras_commands_h
#define _ROS_rufus_master_bras_commands_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rufus_master
{

  class bras_commands : public ros::Msg
  {
    public:
      typedef float _q1_type;
      _q1_type q1;
      typedef float _q2_type;
      _q2_type q2;
      typedef float _q3_type;
      _q3_type q3;
      typedef bool _effector_type;
      _effector_type effector;
      typedef bool _mode_type;
      _mode_type mode;

    bras_commands():
      q1(0),
      q2(0),
      q3(0),
      effector(0),
      mode(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_q1;
      u_q1.real = this->q1;
      *(outbuffer + offset + 0) = (u_q1.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_q1.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_q1.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_q1.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->q1);
      union {
        float real;
        uint32_t base;
      } u_q2;
      u_q2.real = this->q2;
      *(outbuffer + offset + 0) = (u_q2.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_q2.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_q2.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_q2.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->q2);
      union {
        float real;
        uint32_t base;
      } u_q3;
      u_q3.real = this->q3;
      *(outbuffer + offset + 0) = (u_q3.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_q3.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_q3.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_q3.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->q3);
      union {
        bool real;
        uint8_t base;
      } u_effector;
      u_effector.real = this->effector;
      *(outbuffer + offset + 0) = (u_effector.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->effector);
      union {
        bool real;
        uint8_t base;
      } u_mode;
      u_mode.real = this->mode;
      *(outbuffer + offset + 0) = (u_mode.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mode);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_q1;
      u_q1.base = 0;
      u_q1.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_q1.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_q1.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_q1.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->q1 = u_q1.real;
      offset += sizeof(this->q1);
      union {
        float real;
        uint32_t base;
      } u_q2;
      u_q2.base = 0;
      u_q2.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_q2.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_q2.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_q2.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->q2 = u_q2.real;
      offset += sizeof(this->q2);
      union {
        float real;
        uint32_t base;
      } u_q3;
      u_q3.base = 0;
      u_q3.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_q3.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_q3.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_q3.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->q3 = u_q3.real;
      offset += sizeof(this->q3);
      union {
        bool real;
        uint8_t base;
      } u_effector;
      u_effector.base = 0;
      u_effector.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->effector = u_effector.real;
      offset += sizeof(this->effector);
      union {
        bool real;
        uint8_t base;
      } u_mode;
      u_mode.base = 0;
      u_mode.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->mode = u_mode.real;
      offset += sizeof(this->mode);
     return offset;
    }

    virtual const char * getType() override { return "rufus_master/bras_commands"; };
    virtual const char * getMD5() override { return "c601fa504cc9d6a4ab9d6470a7e644ab"; };

  };

}
#endif
