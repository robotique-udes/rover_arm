#ifndef _ROS_rover_arm_arm_gui_cmd_h
#define _ROS_rover_arm_arm_gui_cmd_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rover_arm
{

  class arm_gui_cmd : public ros::Msg
  {
    public:
      bool enable[4];
      typedef uint8_t _state_type;
      _state_type state;

    arm_gui_cmd():
      enable(),
      state(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 4; i++){
      union {
        bool real;
        uint8_t base;
      } u_enablei;
      u_enablei.real = this->enable[i];
      *(outbuffer + offset + 0) = (u_enablei.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->enable[i]);
      }
      *(outbuffer + offset + 0) = (this->state >> (8 * 0)) & 0xFF;
      offset += sizeof(this->state);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 4; i++){
      union {
        bool real;
        uint8_t base;
      } u_enablei;
      u_enablei.base = 0;
      u_enablei.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->enable[i] = u_enablei.real;
      offset += sizeof(this->enable[i]);
      }
      this->state =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->state);
     return offset;
    }

    virtual const char * getType() override { return "rover_arm/arm_gui_cmd"; };
    virtual const char * getMD5() override { return "c39daca3a7fb7f1968c73e114c0cb8d5"; };

  };

}
#endif
