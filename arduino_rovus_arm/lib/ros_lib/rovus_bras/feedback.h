#ifndef _ROS_rovus_bras_feedback_h
#define _ROS_rovus_bras_feedback_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rovus_bras
{

  class feedback : public ros::Msg
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
      typedef bool _singular_matrix_type;
      _singular_matrix_type singular_matrix;
      typedef float _m1_type;
      _m1_type m1;
      typedef float _m2_type;
      _m2_type m2;
      typedef float _m3_type;
      _m3_type m3;
      typedef float _m4_type;
      _m4_type m4;
      typedef bool _ctrl_mode_type;
      _ctrl_mode_type ctrl_mode;
      typedef int8_t _current_joint_type;
      _current_joint_type current_joint;
      typedef float _speed_multiplier_type;
      _speed_multiplier_type speed_multiplier;
      typedef bool _limiteur_type;
      _limiteur_type limiteur;
      typedef bool _calibration_type;
      _calibration_type calibration;

    feedback():
      j1(0),
      j2(0),
      j3(0),
      j4(0),
      singular_matrix(0),
      m1(0),
      m2(0),
      m3(0),
      m4(0),
      ctrl_mode(0),
      current_joint(0),
      speed_multiplier(0),
      limiteur(0),
      calibration(0)
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
      union {
        bool real;
        uint8_t base;
      } u_singular_matrix;
      u_singular_matrix.real = this->singular_matrix;
      *(outbuffer + offset + 0) = (u_singular_matrix.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->singular_matrix);
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
      } u_ctrl_mode;
      u_ctrl_mode.real = this->ctrl_mode;
      *(outbuffer + offset + 0) = (u_ctrl_mode.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->ctrl_mode);
      union {
        int8_t real;
        uint8_t base;
      } u_current_joint;
      u_current_joint.real = this->current_joint;
      *(outbuffer + offset + 0) = (u_current_joint.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->current_joint);
      union {
        float real;
        uint32_t base;
      } u_speed_multiplier;
      u_speed_multiplier.real = this->speed_multiplier;
      *(outbuffer + offset + 0) = (u_speed_multiplier.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_speed_multiplier.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_speed_multiplier.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_speed_multiplier.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->speed_multiplier);
      union {
        bool real;
        uint8_t base;
      } u_limiteur;
      u_limiteur.real = this->limiteur;
      *(outbuffer + offset + 0) = (u_limiteur.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->limiteur);
      union {
        bool real;
        uint8_t base;
      } u_calibration;
      u_calibration.real = this->calibration;
      *(outbuffer + offset + 0) = (u_calibration.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->calibration);
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
      union {
        bool real;
        uint8_t base;
      } u_singular_matrix;
      u_singular_matrix.base = 0;
      u_singular_matrix.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->singular_matrix = u_singular_matrix.real;
      offset += sizeof(this->singular_matrix);
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
      } u_ctrl_mode;
      u_ctrl_mode.base = 0;
      u_ctrl_mode.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->ctrl_mode = u_ctrl_mode.real;
      offset += sizeof(this->ctrl_mode);
      union {
        int8_t real;
        uint8_t base;
      } u_current_joint;
      u_current_joint.base = 0;
      u_current_joint.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->current_joint = u_current_joint.real;
      offset += sizeof(this->current_joint);
      union {
        float real;
        uint32_t base;
      } u_speed_multiplier;
      u_speed_multiplier.base = 0;
      u_speed_multiplier.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_speed_multiplier.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_speed_multiplier.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_speed_multiplier.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->speed_multiplier = u_speed_multiplier.real;
      offset += sizeof(this->speed_multiplier);
      union {
        bool real;
        uint8_t base;
      } u_limiteur;
      u_limiteur.base = 0;
      u_limiteur.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->limiteur = u_limiteur.real;
      offset += sizeof(this->limiteur);
      union {
        bool real;
        uint8_t base;
      } u_calibration;
      u_calibration.base = 0;
      u_calibration.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->calibration = u_calibration.real;
      offset += sizeof(this->calibration);
     return offset;
    }

    virtual const char * getType() override { return "rovus_bras/feedback"; };
    virtual const char * getMD5() override { return "2254bad58b57902bcd694e2ebca15e8d"; };

  };

}
#endif
