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
      typedef float _m1_Period_type;
      _m1_Period_type m1_Period;
      typedef bool _m1_Dir_type;
      _m1_Dir_type m1_Dir;
      typedef bool _m1_En_type;
      _m1_En_type m1_En;
      typedef float _m2_Period_type;
      _m2_Period_type m2_Period;
      typedef bool _m2_Dir_type;
      _m2_Dir_type m2_Dir;
      typedef bool _m2_En_type;
      _m2_En_type m2_En;
      typedef float _m3_Period_type;
      _m3_Period_type m3_Period;
      typedef bool _m3_Dir_type;
      _m3_Dir_type m3_Dir;
      typedef bool _m3_En_type;
      _m3_En_type m3_En;
      typedef float _m4_Period_type;
      _m4_Period_type m4_Period;
      typedef bool _m4_Dir_type;
      _m4_Dir_type m4_Dir;
      typedef bool _m4_En_type;
      _m4_En_type m4_En;
      typedef bool _calib_type;
      _calib_type calib;

    vitesse_moteur_msg():
      m1_Period(0),
      m1_Dir(0),
      m1_En(0),
      m2_Period(0),
      m2_Dir(0),
      m2_En(0),
      m3_Period(0),
      m3_Dir(0),
      m3_En(0),
      m4_Period(0),
      m4_Dir(0),
      m4_En(0),
      calib(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_m1_Period;
      u_m1_Period.real = this->m1_Period;
      *(outbuffer + offset + 0) = (u_m1_Period.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m1_Period.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m1_Period.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m1_Period.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m1_Period);
      union {
        bool real;
        uint8_t base;
      } u_m1_Dir;
      u_m1_Dir.real = this->m1_Dir;
      *(outbuffer + offset + 0) = (u_m1_Dir.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->m1_Dir);
      union {
        bool real;
        uint8_t base;
      } u_m1_En;
      u_m1_En.real = this->m1_En;
      *(outbuffer + offset + 0) = (u_m1_En.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->m1_En);
      union {
        float real;
        uint32_t base;
      } u_m2_Period;
      u_m2_Period.real = this->m2_Period;
      *(outbuffer + offset + 0) = (u_m2_Period.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m2_Period.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m2_Period.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m2_Period.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m2_Period);
      union {
        bool real;
        uint8_t base;
      } u_m2_Dir;
      u_m2_Dir.real = this->m2_Dir;
      *(outbuffer + offset + 0) = (u_m2_Dir.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->m2_Dir);
      union {
        bool real;
        uint8_t base;
      } u_m2_En;
      u_m2_En.real = this->m2_En;
      *(outbuffer + offset + 0) = (u_m2_En.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->m2_En);
      union {
        float real;
        uint32_t base;
      } u_m3_Period;
      u_m3_Period.real = this->m3_Period;
      *(outbuffer + offset + 0) = (u_m3_Period.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m3_Period.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m3_Period.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m3_Period.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m3_Period);
      union {
        bool real;
        uint8_t base;
      } u_m3_Dir;
      u_m3_Dir.real = this->m3_Dir;
      *(outbuffer + offset + 0) = (u_m3_Dir.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->m3_Dir);
      union {
        bool real;
        uint8_t base;
      } u_m3_En;
      u_m3_En.real = this->m3_En;
      *(outbuffer + offset + 0) = (u_m3_En.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->m3_En);
      union {
        float real;
        uint32_t base;
      } u_m4_Period;
      u_m4_Period.real = this->m4_Period;
      *(outbuffer + offset + 0) = (u_m4_Period.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_m4_Period.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_m4_Period.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_m4_Period.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->m4_Period);
      union {
        bool real;
        uint8_t base;
      } u_m4_Dir;
      u_m4_Dir.real = this->m4_Dir;
      *(outbuffer + offset + 0) = (u_m4_Dir.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->m4_Dir);
      union {
        bool real;
        uint8_t base;
      } u_m4_En;
      u_m4_En.real = this->m4_En;
      *(outbuffer + offset + 0) = (u_m4_En.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->m4_En);
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
      } u_m1_Period;
      u_m1_Period.base = 0;
      u_m1_Period.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m1_Period.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m1_Period.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m1_Period.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m1_Period = u_m1_Period.real;
      offset += sizeof(this->m1_Period);
      union {
        bool real;
        uint8_t base;
      } u_m1_Dir;
      u_m1_Dir.base = 0;
      u_m1_Dir.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->m1_Dir = u_m1_Dir.real;
      offset += sizeof(this->m1_Dir);
      union {
        bool real;
        uint8_t base;
      } u_m1_En;
      u_m1_En.base = 0;
      u_m1_En.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->m1_En = u_m1_En.real;
      offset += sizeof(this->m1_En);
      union {
        float real;
        uint32_t base;
      } u_m2_Period;
      u_m2_Period.base = 0;
      u_m2_Period.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m2_Period.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m2_Period.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m2_Period.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m2_Period = u_m2_Period.real;
      offset += sizeof(this->m2_Period);
      union {
        bool real;
        uint8_t base;
      } u_m2_Dir;
      u_m2_Dir.base = 0;
      u_m2_Dir.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->m2_Dir = u_m2_Dir.real;
      offset += sizeof(this->m2_Dir);
      union {
        bool real;
        uint8_t base;
      } u_m2_En;
      u_m2_En.base = 0;
      u_m2_En.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->m2_En = u_m2_En.real;
      offset += sizeof(this->m2_En);
      union {
        float real;
        uint32_t base;
      } u_m3_Period;
      u_m3_Period.base = 0;
      u_m3_Period.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m3_Period.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m3_Period.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m3_Period.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m3_Period = u_m3_Period.real;
      offset += sizeof(this->m3_Period);
      union {
        bool real;
        uint8_t base;
      } u_m3_Dir;
      u_m3_Dir.base = 0;
      u_m3_Dir.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->m3_Dir = u_m3_Dir.real;
      offset += sizeof(this->m3_Dir);
      union {
        bool real;
        uint8_t base;
      } u_m3_En;
      u_m3_En.base = 0;
      u_m3_En.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->m3_En = u_m3_En.real;
      offset += sizeof(this->m3_En);
      union {
        float real;
        uint32_t base;
      } u_m4_Period;
      u_m4_Period.base = 0;
      u_m4_Period.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_m4_Period.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_m4_Period.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_m4_Period.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->m4_Period = u_m4_Period.real;
      offset += sizeof(this->m4_Period);
      union {
        bool real;
        uint8_t base;
      } u_m4_Dir;
      u_m4_Dir.base = 0;
      u_m4_Dir.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->m4_Dir = u_m4_Dir.real;
      offset += sizeof(this->m4_Dir);
      union {
        bool real;
        uint8_t base;
      } u_m4_En;
      u_m4_En.base = 0;
      u_m4_En.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->m4_En = u_m4_En.real;
      offset += sizeof(this->m4_En);
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
    virtual const char * getMD5() override { return "7cd4f6c3688597f156ba88462afe6362"; };

  };

}
#endif
