#ifndef _ROS_rovus_bras_angle_h
#define _ROS_rovus_bras_angle_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rovus_bras
{

  class angle : public ros::Msg
  {
    public:
      typedef float _j1_type;
      _j1_type j1;
      typedef float _j2_type;
      _j2_type j2;
      typedef float _j3_type;
      _j3_type j3;
      typedef float _j4_type;
      _j4_type j4;

    angle():
      j1(0),
      j2(0),
      j3(0),
      j4(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_j1;
      u_j1.real = this->j1;
      *(outbuffer + offset + 0) = (u_j1.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_j1.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_j1.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_j1.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->j1);
      union {
        float real;
        uint32_t base;
      } u_j2;
      u_j2.real = this->j2;
      *(outbuffer + offset + 0) = (u_j2.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_j2.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_j2.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_j2.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->j2);
      union {
        float real;
        uint32_t base;
      } u_j3;
      u_j3.real = this->j3;
      *(outbuffer + offset + 0) = (u_j3.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_j3.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_j3.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_j3.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->j3);
      union {
        float real;
        uint32_t base;
      } u_j4;
      u_j4.real = this->j4;
      *(outbuffer + offset + 0) = (u_j4.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_j4.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_j4.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_j4.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->j4);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_j1;
      u_j1.base = 0;
      u_j1.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_j1.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_j1.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_j1.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->j1 = u_j1.real;
      offset += sizeof(this->j1);
      union {
        float real;
        uint32_t base;
      } u_j2;
      u_j2.base = 0;
      u_j2.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_j2.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_j2.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_j2.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->j2 = u_j2.real;
      offset += sizeof(this->j2);
      union {
        float real;
        uint32_t base;
      } u_j3;
      u_j3.base = 0;
      u_j3.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_j3.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_j3.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_j3.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->j3 = u_j3.real;
      offset += sizeof(this->j3);
      union {
        float real;
        uint32_t base;
      } u_j4;
      u_j4.base = 0;
      u_j4.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_j4.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_j4.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_j4.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->j4 = u_j4.real;
      offset += sizeof(this->j4);
     return offset;
    }

    virtual const char * getType() override { return "rovus_bras/angle"; };
    virtual const char * getMD5() override { return "5585c74950e7d793840b3e8036e913a6"; };

  };

}
#endif
