#ifndef _ROS_arm_us_msg_GraphInfo_h
#define _ROS_arm_us_msg_GraphInfo_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace arm_us_msg
{

  class GraphInfo : public ros::Msg
  {
    public:
      uint32_t angle_length;
      typedef float _angle_type;
      _angle_type st_angle;
      _angle_type * angle;

    GraphInfo():
      angle_length(0), st_angle(), angle(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->angle_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->angle_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->angle_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->angle_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angle_length);
      for( uint32_t i = 0; i < angle_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->angle[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t angle_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      angle_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      angle_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      angle_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->angle_length);
      if(angle_lengthT > angle_length)
        this->angle = (float*)realloc(this->angle, angle_lengthT * sizeof(float));
      angle_length = angle_lengthT;
      for( uint32_t i = 0; i < angle_length; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_angle));
        memcpy( &(this->angle[i]), &(this->st_angle), sizeof(float));
      }
     return offset;
    }

    virtual const char * getType() override { return "arm_us_msg/GraphInfo"; };
    virtual const char * getMD5() override { return "724c11f6ab71051633f4d5daecf902c4"; };

  };

}
#endif
