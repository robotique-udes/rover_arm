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
      uint32_t position_length;
      typedef float _position_type;
      _position_type st_position;
      _position_type * position;
      uint32_t velocity_length;
      typedef float _velocity_type;
      _velocity_type st_velocity;
      _velocity_type * velocity;
      uint32_t connected_length;
      typedef bool _connected_type;
      _connected_type st_connected;
      _connected_type * connected;
      uint32_t limit_reached_length;
      typedef bool _limit_reached_type;
      _limit_reached_type st_limit_reached;
      _limit_reached_type * limit_reached;

    GuiInfo():
      position_length(0), st_position(), position(nullptr),
      velocity_length(0), st_velocity(), velocity(nullptr),
      connected_length(0), st_connected(), connected(nullptr),
      limit_reached_length(0), st_limit_reached(), limit_reached(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->position_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->position_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->position_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->position_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->position_length);
      for( uint32_t i = 0; i < position_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->position[i]);
      }
      *(outbuffer + offset + 0) = (this->velocity_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->velocity_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->velocity_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->velocity_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->velocity_length);
      for( uint32_t i = 0; i < velocity_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->velocity[i]);
      }
      *(outbuffer + offset + 0) = (this->connected_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->connected_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->connected_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->connected_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->connected_length);
      for( uint32_t i = 0; i < connected_length; i++){
      union {
        bool real;
        uint8_t base;
      } u_connectedi;
      u_connectedi.real = this->connected[i];
      *(outbuffer + offset + 0) = (u_connectedi.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->connected[i]);
      }
      *(outbuffer + offset + 0) = (this->limit_reached_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->limit_reached_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->limit_reached_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->limit_reached_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->limit_reached_length);
      for( uint32_t i = 0; i < limit_reached_length; i++){
      union {
        bool real;
        uint8_t base;
      } u_limit_reachedi;
      u_limit_reachedi.real = this->limit_reached[i];
      *(outbuffer + offset + 0) = (u_limit_reachedi.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->limit_reached[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t position_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      position_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      position_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      position_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->position_length);
      if(position_lengthT > position_length)
        this->position = (float*)realloc(this->position, position_lengthT * sizeof(float));
      position_length = position_lengthT;
      for( uint32_t i = 0; i < position_length; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_position));
        memcpy( &(this->position[i]), &(this->st_position), sizeof(float));
      }
      uint32_t velocity_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      velocity_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      velocity_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      velocity_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->velocity_length);
      if(velocity_lengthT > velocity_length)
        this->velocity = (float*)realloc(this->velocity, velocity_lengthT * sizeof(float));
      velocity_length = velocity_lengthT;
      for( uint32_t i = 0; i < velocity_length; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_velocity));
        memcpy( &(this->velocity[i]), &(this->st_velocity), sizeof(float));
      }
      uint32_t connected_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      connected_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      connected_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      connected_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->connected_length);
      if(connected_lengthT > connected_length)
        this->connected = (bool*)realloc(this->connected, connected_lengthT * sizeof(bool));
      connected_length = connected_lengthT;
      for( uint32_t i = 0; i < connected_length; i++){
      union {
        bool real;
        uint8_t base;
      } u_st_connected;
      u_st_connected.base = 0;
      u_st_connected.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->st_connected = u_st_connected.real;
      offset += sizeof(this->st_connected);
        memcpy( &(this->connected[i]), &(this->st_connected), sizeof(bool));
      }
      uint32_t limit_reached_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      limit_reached_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      limit_reached_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      limit_reached_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->limit_reached_length);
      if(limit_reached_lengthT > limit_reached_length)
        this->limit_reached = (bool*)realloc(this->limit_reached, limit_reached_lengthT * sizeof(bool));
      limit_reached_length = limit_reached_lengthT;
      for( uint32_t i = 0; i < limit_reached_length; i++){
      union {
        bool real;
        uint8_t base;
      } u_st_limit_reached;
      u_st_limit_reached.base = 0;
      u_st_limit_reached.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->st_limit_reached = u_st_limit_reached.real;
      offset += sizeof(this->st_limit_reached);
        memcpy( &(this->limit_reached[i]), &(this->st_limit_reached), sizeof(bool));
      }
     return offset;
    }

    virtual const char * getType() override { return "arm_us_msg/GuiInfo"; };
    virtual const char * getMD5() override { return "62fa6a65e41e3a61c0e6dd8e2fdea310"; };

  };

}
#endif
