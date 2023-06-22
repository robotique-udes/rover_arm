#ifndef _ROS_arm_us_msg_GuiInfo_h
#define _ROS_arm_us_msg_GuiInfo_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace arm_us_msg
{

  class GuiInfo : public ros::Msg
  {
    public:
      typedef int8_t _current_joint_type;
      _current_joint_type current_joint;
      typedef int8_t _current_mode_type;
      _current_mode_type current_mode;

    GuiInfo():
      current_joint(0),
      current_mode(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_current_joint;
      u_current_joint.real = this->current_joint;
      *(outbuffer + offset + 0) = (u_current_joint.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->current_joint);
      union {
        int8_t real;
        uint8_t base;
      } u_current_mode;
      u_current_mode.real = this->current_mode;
      *(outbuffer + offset + 0) = (u_current_mode.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->current_mode);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_current_joint;
      u_current_joint.base = 0;
      u_current_joint.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->current_joint = u_current_joint.real;
      offset += sizeof(this->current_joint);
      union {
        int8_t real;
        uint8_t base;
      } u_current_mode;
      u_current_mode.base = 0;
      u_current_mode.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->current_mode = u_current_mode.real;
      offset += sizeof(this->current_mode);
     return offset;
    }

    virtual const char * getType() override { return "arm_us_msg/GuiInfo"; };
    virtual const char * getMD5() override { return "d203c3953986637c197c58bd16da36fb"; };

  };

}
#endif
