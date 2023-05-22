#include "JointController.hpp"

rover_arm_lib::JointController::JointController(ros::NodeHandle *p_nodehandle,
                                                double kp, double ki, double kd,
                                                const char *name_pub_joint_state,
                                                DCMotor *p_dc_motor) : pid(kp, ki, kd),
                                                                       pub_joint_state(name_pub_joint_state, &joint_state_msg)
{
    n = p_nodehandle;
    dc_motor = p_dc_motor;

    n->advertise(pub_joint_state);

    setPIDLimits(0, 255);
    setPIDRate(5);
}
