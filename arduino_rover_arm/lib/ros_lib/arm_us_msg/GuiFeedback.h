#ifndef _ROS_arm_us_msg_GuiFeedback_h
#define _ROS_arm_us_msg_GuiFeedback_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace arm_us_msg
{

  class GuiFeedback : public ros::Msg
  {
    public:
      typedef bool _cartesian_type;
      _cartesian_type cartesian;
      typedef bool _joint_type;
      _joint_type joint;
      typedef int8_t _joint_controlled_type;
      _joint_controlled_type joint_controlled;

    GuiFeedback():
      cartesian(0),
      joint(0),
      joint_controlled(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_cartesian;
      u_cartesian.real = this->cartesian;
      *(outbuffer + offset + 0) = (u_cartesian.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->cartesian);
      union {
        bool real;
        uint8_t base;
      } u_joint;
      u_joint.real = this->joint;
      *(outbuffer + offset + 0) = (u_joint.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->joint);
      union {
        int8_t real;
        uint8_t base;
      } u_joint_controlled;
      u_joint_controlled.real = this->joint_controlled;
      *(outbuffer + offset + 0) = (u_joint_controlled.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->joint_controlled);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_cartesian;
      u_cartesian.base = 0;
      u_cartesian.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->cartesian = u_cartesian.real;
      offset += sizeof(this->cartesian);
      union {
        bool real;
        uint8_t base;
      } u_joint;
      u_joint.base = 0;
      u_joint.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->joint = u_joint.real;
      offset += sizeof(this->joint);
      union {
        int8_t real;
        uint8_t base;
      } u_joint_controlled;
      u_joint_controlled.base = 0;
      u_joint_controlled.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->joint_controlled = u_joint_controlled.real;
      offset += sizeof(this->joint_controlled);
     return offset;
    }

    virtual const char * getType() override { return "arm_us_msg/GuiFeedback"; };
    virtual const char * getMD5() override { return "3e4cd23164c9f444702c5af87708c4a3"; };

  };

}
#endif
