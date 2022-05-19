#ifndef _ROS_rovus_bras_vitesse_moteur_msg_h
#define _ROS_rovus_bras_vitesse_moteur_msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rovus_bras
{

  class vitesse_moteur_msg : public ros::Msg
  {
    public:
      typedef float _m1_type;
      _m1_type m1;
      typedef float _m2_type;
      _m2_type m2;
      typedef float _m3_type;
      _m3_type m3;
      typedef float _m4_type;
      _m4_type m4;
      typedef bool _calib_type;
      _calib_type calib;

    vitesse_moteur_msg():
      m1(0),
      m2(0),
      m3(0),
      m4(0),
      calib(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_m1;
      u_m1.real = this->m1;
      *(outbuffer + offset + 0) = (u_m1.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m1.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m1.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m1.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m1);
      union {
        float real;
        uint32_t base;
      } u_m2;
      u_m2.real = this->m2;
      *(outbuffer + offset + 0) = (u_m2.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m2.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m2.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m2.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m2);
      union {
        float real;
        uint32_t base;
      } u_m3;
      u_m3.real = this->m3;
      *(outbuffer + offset + 0) = (u_m3.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m3.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m3.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m3.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m3);
      union {
        float real;
        uint32_t base;
      } u_m4;
      u_m4.real = this->m4;
      *(outbuffer + offset + 0) = (u_m4.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m4.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m4.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m4.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m4);
      union {
        bool real;
        uint8_t base;
      } u_calib;
      u_calib.real = this->calib;
      *(outbuffer + offset + 0) = (u_calib.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->calib);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_m1;
      u_m1.base = 0;
      u_m1.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m1.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m1.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m1.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m1 = u_m1.real;
      offset += sizeof(this->m1);
      union {
        float real;
        uint32_t base;
      } u_m2;
      u_m2.base = 0;
      u_m2.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m2.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m2.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m2.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m2 = u_m2.real;
      offset += sizeof(this->m2);
      union {
        float real;
        uint32_t base;
      } u_m3;
      u_m3.base = 0;
      u_m3.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m3.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m3.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m3.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m3 = u_m3.real;
      offset += sizeof(this->m3);
      union {
        float real;
        uint32_t base;
      } u_m4;
      u_m4.base = 0;
      u_m4.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m4.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m4.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m4.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m4 = u_m4.real;
      offset += sizeof(this->m4);
      union {
        bool real;
        uint8_t base;
      } u_calib;
      u_calib.base = 0;
      u_calib.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->calib = u_calib.real;
      offset += sizeof(this->calib);
     return offset;
    }

    virtual const char * getType() override { return "rovus_bras/vitesse_moteur_msg"; };
    virtual const char * getMD5() override { return "22c1ed3801c986a92bc2c8a8a29d4db5"; };

  };

}
#endif
