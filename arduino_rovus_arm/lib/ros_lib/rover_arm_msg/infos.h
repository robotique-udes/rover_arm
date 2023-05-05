#ifndef _ROS_rover_arm_msg_infos_h
#define _ROS_rover_arm_msg_infos_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rover_arm_msg
{

  class infos : public ros::Msg
  {
    public:
      typedef float _curr_speed_type;
      _curr_speed_type curr_speed;
      float pid_coeffs[3];
      typedef float _command_type;
      _command_type command;

    infos():
      curr_speed(0),
      pid_coeffs(),
      command(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_curr_speed;
      u_curr_speed.real = this->curr_speed;
      *(outbuffer + offset + 0) = (u_curr_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_curr_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_curr_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_curr_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->curr_speed);
      for( uint32_t i = 0; i < 3; i++){
      union {
        float real;
        uint32_t base;
      } u_pid_coeffsi;
      u_pid_coeffsi.real = this->pid_coeffs[i];
      *(outbuffer + offset + 0) = (u_pid_coeffsi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_pid_coeffsi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_pid_coeffsi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_pid_coeffsi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->pid_coeffs[i]);
      }
      union {
        float real;
        uint32_t base;
      } u_command;
      u_command.real = this->command;
      *(outbuffer + offset + 0) = (u_command.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_command.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_command.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_command.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->command);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_curr_speed;
      u_curr_speed.base = 0;
      u_curr_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_curr_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_curr_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_curr_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->curr_speed = u_curr_speed.real;
      offset += sizeof(this->curr_speed);
      for( uint32_t i = 0; i < 3; i++){
      union {
        float real;
        uint32_t base;
      } u_pid_coeffsi;
      u_pid_coeffsi.base = 0;
      u_pid_coeffsi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_pid_coeffsi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_pid_coeffsi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_pid_coeffsi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->pid_coeffs[i] = u_pid_coeffsi.real;
      offset += sizeof(this->pid_coeffs[i]);
      }
      union {
        float real;
        uint32_t base;
      } u_command;
      u_command.base = 0;
      u_command.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_command.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_command.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_command.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->command = u_command.real;
      offset += sizeof(this->command);
     return offset;
    }

    virtual const char * getType() override { return "rover_arm_msg/infos"; };
    virtual const char * getMD5() override { return "4a7e8c54d92107ed4d9af4c65816b0be"; };

  };

}
#endif
