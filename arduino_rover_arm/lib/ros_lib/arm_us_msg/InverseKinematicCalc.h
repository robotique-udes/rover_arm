#ifndef _ROS_SERVICE_InverseKinematicCalc_h
#define _ROS_SERVICE_InverseKinematicCalc_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace arm_us_msg
{

static const char INVERSEKINEMATICCALC[] = "arm_us_msg/InverseKinematicCalc";

  class InverseKinematicCalcRequest : public ros::Msg
  {
    public:
      float angles[3];
      float commands[3];

    InverseKinematicCalcRequest():
      angles(),
      commands()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 3; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->angles[i]);
      }
      for( uint32_t i = 0; i < 3; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->commands[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 3; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->angles[i]));
      }
      for( uint32_t i = 0; i < 3; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->commands[i]));
      }
     return offset;
    }

    virtual const char * getType() override { return INVERSEKINEMATICCALC; };
    virtual const char * getMD5() override { return "28ddc5d03fa06efd16a8651045a43b27"; };

  };

  class InverseKinematicCalcResponse : public ros::Msg
  {
    public:
      float velocities[3];
      typedef bool _singularMatrix_type;
      _singularMatrix_type singularMatrix;

    InverseKinematicCalcResponse():
      velocities(),
      singularMatrix(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 3; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->velocities[i]);
      }
      union {
        bool real;
        uint8_t base;
      } u_singularMatrix;
      u_singularMatrix.real = this->singularMatrix;
      *(outbuffer + offset + 0) = (u_singularMatrix.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->singularMatrix);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 3; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->velocities[i]));
      }
      union {
        bool real;
        uint8_t base;
      } u_singularMatrix;
      u_singularMatrix.base = 0;
      u_singularMatrix.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->singularMatrix = u_singularMatrix.real;
      offset += sizeof(this->singularMatrix);
     return offset;
    }

    virtual const char * getType() override { return INVERSEKINEMATICCALC; };
    virtual const char * getMD5() override { return "0a33c3de9104c55e6d543d797c0d1dc9"; };

  };

  class InverseKinematicCalc {
    public:
    typedef InverseKinematicCalcRequest Request;
    typedef InverseKinematicCalcResponse Response;
  };

}
#endif
