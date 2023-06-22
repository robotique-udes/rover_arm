#ifndef _ROS_arm_us_msg_JointLimits_h
#define _ROS_arm_us_msg_JointLimits_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace arm_us_msg
{

  class JointLimits : public ros::Msg
  {
    public:
      uint32_t joint_limits_length;
      typedef bool _joint_limits_type;
      _joint_limits_type st_joint_limits;
      _joint_limits_type * joint_limits;

    JointLimits():
      joint_limits_length(0), st_joint_limits(), joint_limits(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->joint_limits_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->joint_limits_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->joint_limits_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->joint_limits_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->joint_limits_length);
      for( uint32_t i = 0; i < joint_limits_length; i++){
      union {
        bool real;
        uint8_t base;
      } u_joint_limitsi;
      u_joint_limitsi.real = this->joint_limits[i];
      *(outbuffer + offset + 0) = (u_joint_limitsi.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->joint_limits[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t joint_limits_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      joint_limits_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      joint_limits_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      joint_limits_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->joint_limits_length);
      if(joint_limits_lengthT > joint_limits_length)
        this->joint_limits = (bool*)realloc(this->joint_limits, joint_limits_lengthT * sizeof(bool));
      joint_limits_length = joint_limits_lengthT;
      for( uint32_t i = 0; i < joint_limits_length; i++){
      union {
        bool real;
        uint8_t base;
      } u_st_joint_limits;
      u_st_joint_limits.base = 0;
      u_st_joint_limits.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->st_joint_limits = u_st_joint_limits.real;
      offset += sizeof(this->st_joint_limits);
        memcpy( &(this->joint_limits[i]), &(this->st_joint_limits), sizeof(bool));
      }
     return offset;
    }

    virtual const char * getType() override { return "arm_us_msg/JointLimits"; };
    virtual const char * getMD5() override { return "52cee38eb2f0ce5fe0b2ddc876c38710"; };

  };

}
#endif
