#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "rover_arm_msgs/joint_state.h"
#include <thread>
#include "rover_arm_msgs/diff_kinematics_calc.h"
#include "rover_arm_msgs/feedback.h"
#include "std_msgs/Empty.h"
#include "sensor_msgs/JointState.h"

#define IN
#define OUT

#define NB_JOINT 6
#define NB_JOINT_CARTESIAN_MODE 3
#define LOCK std::lock_guard<std::mutex>

struct Keybinds
{
    int axis_cmd_x;
    int axis_cmd_y;
    int axis_cmd_z;
    int axis_cmd_joint;
    int axis_cmd_gripper_rot;
    int axis_cmd_gripper_grip;

    int button_cmd_a_positive;
    int button_cmd_a_negative;
    int button_crawler;
    int button_enable;
    int button_fast;
    int button_joint_increase;
    int button_joint_decrease;
    int button_control_mode_toggle;
};

struct SpeedModes
{
    float crawler = 0.0f;
    float normal = 0.0f;
    float fast = 0.0f;
    float speed_cartesian_mode_divider = 1.0f;
};

class ArmMasterNode
{
public:
    // =========================================================================
    // Constructor / Destructor
    // =========================================================================
    ArmMasterNode() : m_prev_msg()
    {
        getParams();

        m_sub_heartbeat = m_nh.subscribe<std_msgs::Empty>("/base_heartbeat", 1, &ArmMasterNode::CBHeartbeat, this);
        m_timer_watchdog = m_nh.createTimer(ros::Duration(1.0f), &ArmMasterNode::CBResetWatchdog, this);

        // Initialising publisher for DesiredJointState
        for (int i = 0; i < NB_JOINT; i++)
        {
            m_apub_j[i] = m_nh.advertise<rover_arm_msgs::joint_state>("/arm/j" + std::to_string(i) + "/DJS", 1);
        }

        // Initialising subscriber for JointState
        for (int i = 0; i < NB_JOINT; i++)
        {
            m_asub_j[i] = m_nh.subscribe<rover_arm_msgs::joint_state>("/arm/j" + std::to_string(i) + "/JS",
                                                                      1,
                                                                      boost::bind(&ArmMasterNode::CBJS, this, _1, i));
        }

        // Initialising publisher for dynamixels
        m_pub_dynamixel = m_nh.advertise<sensor_msgs::JointState>("/desired_joint_states", 1);

        // Initialising joy subscriber
        // Needs to initialise with column of zeros otherwise indexes will be
        // out of bound in CBJoy and cause a segmentation fault
        m_prev_msg.buttons = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        m_prev_msg.axes = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                           0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        m_sub_joy = m_nh.subscribe<sensor_msgs::Joy>("/arm_joy", 1, &ArmMasterNode::CBJoy, this);

        // Initialising service client for inverse differential kinematic calculations
        m_srv_diff_calc = m_nh.serviceClient<rover_arm_msgs::diff_kinematics_calcRequest,
                                             rover_arm_msgs::diff_kinematics_calcResponse>("diff_kinematics_calc");

        // Initialising gui feedback msgs. It comes from an older version of
        // rover_arm and needs to be cleaned (multiple ununsed members)). It's
        // link to a timer but doesn't start on it's own. It needs to be started
        // after we start spinning. The / before the topic name means it will be
        // a global topic (not affected by namespaces)
        m_pub_feedback = m_nh.advertise<rover_arm_msgs::feedback>("/rover_arm_feedback", 1);
        m_timer_feedback = m_nh.createTimer(ros::Duration(0.1), &ArmMasterNode::CBFeedback, this, false, false);

        ROS_INFO("%s node is ready", ros::this_node::getName().c_str());
    }

    // =========================================================================
    // Public methods
    // =========================================================================
    void Run()
    {
        ros::AsyncSpinner spinner(0); // Number of thread used = number of cores
        spinner.start();
        m_timer_feedback.start();
        ros::waitForShutdown();
        m_timer_feedback.stop();
    }

private:
    // =========================================================================
    // Private members
    // =========================================================================
    ros::NodeHandle m_nh;
    ros::Subscriber m_sub_heartbeat;
    ros::Subscriber m_sub_joy;
    ros::Subscriber m_asub_j[NB_JOINT];
    ros::Publisher m_apub_j[NB_JOINT];
    ros::Publisher m_pub_dynamixel;

    ros::ServiceClient m_srv_diff_calc;
    ros::Publisher m_pub_feedback;
    ros::Timer m_timer_feedback;
    ros::Timer m_timer_watchdog;

    std::mutex m_mutex_m_b_com_is_alive;
    bool m_b_com_is_alive = false;
    std::mutex m_mutex_m_b_watchdog_is_alive;
    bool m_b_watchdog_is_alive = false;

    std::mutex m_mutex_m_af_angle;
    float m_af_angle[NB_JOINT] = {10.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f};

    std::mutex m_mutex_m_af_speed;
    float m_af_speed[NB_JOINT] = {0.0f};

    std::mutex m_mutex_m_b_mode_cartesian;
    bool m_b_mode_cartesian = false;

    std::mutex m_mutex_m_n_current_joint;
    int m_n_current_joint = 0;

    Keybinds m_keybind;
    SpeedModes m_speed_modes;
    sensor_msgs::Joy m_prev_msg;
    float m_af_joint_max_speed[NB_JOINT_CARTESIAN_MODE];

    // =========================================================================
    // Private methods
    // =========================================================================
    // Magic happens here
    void CBJoy(const sensor_msgs::Joy msg)
    {
        // Ctrl mode toggling
        // When releasing control_mode_toggle_button
        if (!msg.buttons[m_keybind.button_control_mode_toggle] &&
            m_prev_msg.buttons[m_keybind.button_control_mode_toggle])
        {
            LOCK lock(m_mutex_m_b_mode_cartesian);
            m_b_mode_cartesian = !m_b_mode_cartesian;
        }

        // Current joint update (scope is for lock's destructor)
        {
            LOCK lock(m_mutex_m_n_current_joint);
            if (msg.buttons[m_keybind.button_joint_increase] && !m_prev_msg.buttons[m_keybind.button_joint_increase])
            {
                m_n_current_joint = ((m_n_current_joint + 1) >= NB_JOINT) ? 0 : m_n_current_joint + 1;
            }
            if (msg.buttons[m_keybind.button_joint_decrease] && !m_prev_msg.buttons[m_keybind.button_joint_decrease])
            {
                m_n_current_joint = ((m_n_current_joint - 1) < 0) ? NB_JOINT - 1 : m_n_current_joint - 1;
            }
        }

        // Gets which speed mode is selected
        float f_speed_factor = 0.0f;
        if (msg.buttons[m_keybind.button_crawler])
        {
            f_speed_factor = m_speed_modes.crawler;
        }
        else if (msg.buttons[m_keybind.button_fast])
        {
            f_speed_factor = m_speed_modes.fast;
        }
        else
        {
            f_speed_factor = m_speed_modes.normal;
        }

        // Initialise msgs and does the necessary calculation
        rover_arm_msgs::joint_state motor_cmd[NB_JOINT];
        sensor_msgs::JointState motor_cmd_dynamixel;
        motor_cmd_dynamixel.name = {"gripper_rotation", "gripper_grip"};
        motor_cmd_dynamixel.velocity = {0.0f, 0.0f};

        if (msg.buttons[m_keybind.button_enable] && m_b_watchdog_is_alive)
        {
            if (m_b_mode_cartesian && m_srv_diff_calc.exists())
            {
                // Build request for call
                rover_arm_msgs::diff_kinematics_calcRequest request;
                for (int i = 0; i < NB_JOINT_CARTESIAN_MODE; i++)
                {
                    LOCK lock(m_mutex_m_af_angle);
                    request.angles[i] = m_af_angle[i];
                }
                request.cmd[0] = msg.axes[m_keybind.axis_cmd_x];
                request.cmd[1] = msg.axes[m_keybind.axis_cmd_y];
                request.cmd[2] = msg.axes[m_keybind.axis_cmd_z];

                // Calling service
                rover_arm_msgs::diff_kinematics_calcResponse reponse;
                if (m_srv_diff_calc.call(request, reponse))
                {
                    if (reponse.singularMatrix)
                    {
                        ROS_WARN("Singular Matrix; Jog in joint");
                    }
                    else
                    {
                        for (int i = 0; i < NB_JOINT_CARTESIAN_MODE; i++)
                        {
                            motor_cmd[i].speed = (reponse.vitesses[i] * f_speed_factor) /
                                                 m_speed_modes.speed_cartesian_mode_divider;
                        }

                        // 
                        motor_cmd[3].speed = -motor_cmd[1].speed;

                        float f_speed_limitor_factor = 1.0f;
                        for (int i = 0; i < NB_JOINT_CARTESIAN_MODE + 1; i++)
                        {
                            f_speed_factor = abs(m_af_joint_max_speed[i] / motor_cmd[i].speed);
                            f_speed_limitor_factor = f_speed_factor < f_speed_limitor_factor ? f_speed_factor : f_speed_limitor_factor;
                        }

                        if (f_speed_limitor_factor < 1.0f)
                        {
                            for (int i = 0; i < NB_JOINT_CARTESIAN_MODE; i++)
                            {
                                motor_cmd[i].speed = motor_cmd[i].speed * f_speed_limitor_factor;
                            }
                        }
                    }
                }
                else
                {
                    ROS_ERROR("Failed to call diff_calc service, jog in cartesian not available");
                }
            }
            else // Joint Mode
            {
                motor_cmd[m_n_current_joint].speed = f_speed_factor * msg.axes[m_keybind.axis_cmd_joint];
            }

            // Last 3 joints
            if (msg.buttons[m_keybind.button_cmd_a_positive])
            {
                motor_cmd[3].speed += m_speed_modes.normal;
            }
            else if (msg.buttons[m_keybind.button_cmd_a_negative])
            {
                motor_cmd[3].speed -= m_speed_modes.normal;
            }

            motor_cmd_dynamixel.velocity[0] = m_speed_modes.normal * msg.axes[m_keybind.axis_cmd_gripper_rot];
            motor_cmd_dynamixel.velocity[1] = m_speed_modes.crawler * msg.axes[m_keybind.axis_cmd_gripper_grip];
        }

        // Sending msgs
        for (int i = 0; i < NB_JOINT; i++)
        {
            m_apub_j[i].publish(motor_cmd[i]);
        }

        m_pub_dynamixel.publish(motor_cmd_dynamixel);
        m_prev_msg = msg;
    }

    void CBJS(const rover_arm_msgs::joint_stateConstPtr &msg, int index)
    {
        LOCK lock(m_mutex_m_af_angle);
        m_af_angle[index] = msg->angle;
    }

    void getParams()
    {
        std::string str_param_prefix = ros::this_node::getName() + "/";
        getKeybind(str_param_prefix);
        getSpeedModes(str_param_prefix);
        getJointMaxSpeed(str_param_prefix);
    }

    void getKeybind(std::string str_param_prefix)
    {
        m_keybind.axis_cmd_x = m_nh.param(str_param_prefix + "cmd_x", 1);
        m_keybind.axis_cmd_y = m_nh.param(str_param_prefix + "cmd_y", 3);
        m_keybind.axis_cmd_z = m_nh.param(str_param_prefix + "cmd_z", 0);
        m_keybind.axis_cmd_joint = m_nh.param(str_param_prefix + "cmd_joint", 1);
        m_keybind.axis_cmd_gripper_rot = m_nh.param(str_param_prefix + "cmd_gripper_rot", 6);
        m_keybind.axis_cmd_gripper_grip = m_nh.param(str_param_prefix + "cmd_gripper_grip", 7);

        m_keybind.button_cmd_a_negative = m_nh.param(str_param_prefix + "cmd_a_negative", 4);
        m_keybind.button_cmd_a_positive = m_nh.param(str_param_prefix + "cmd_a_positive", 5);
        m_keybind.button_crawler = m_nh.param(str_param_prefix + "crawler", 1);
        m_keybind.button_enable = m_nh.param(str_param_prefix + "enable", 4);
        m_keybind.button_fast = m_nh.param(str_param_prefix + "fast", 5);
        m_keybind.button_joint_increase = m_nh.param(str_param_prefix + "joint_increase", 2);
        m_keybind.button_joint_decrease = m_nh.param(str_param_prefix + "joint_decrease", 0);
        m_keybind.button_control_mode_toggle = m_nh.param(str_param_prefix + "control_mode_toggle", 3);
        ROS_INFO("Keybind set");
    }

    void getSpeedModes(std::string str_param_prefix)
    {
        m_speed_modes.crawler = m_nh.param(str_param_prefix + "speed_crawler", 0.0f);
        m_speed_modes.normal = m_nh.param(str_param_prefix + "speed_normal", 5.0f);
        m_speed_modes.fast = m_nh.param(str_param_prefix + "speed_fast", 0.0f);
        m_speed_modes.speed_cartesian_mode_divider = m_nh.param(str_param_prefix + "speed_cartesian_mode_divider", 5.0f);
        ROS_INFO("Speeds set");
    }

    void getJointMaxSpeed(std::string str_param_prefix)
    {
        m_af_joint_max_speed[0] = m_nh.param(str_param_prefix + "max_speed_j0", 10.0);
        m_af_joint_max_speed[1] = m_nh.param(str_param_prefix + "max_speed_j1", 10.0);
        m_af_joint_max_speed[2] = m_nh.param(str_param_prefix + "max_speed_j2", 10.0);
        m_af_joint_max_speed[3] = m_nh.param(str_param_prefix + "max_speed_j3", 10.0);
    }

    void CBFeedback(const ros::TimerEvent &event)
    {
        // Building msg
        rover_arm_msgs::feedback msg;

        {
            LOCK lock(m_mutex_m_af_angle);
            for (int i = 0; i < NB_JOINT; i++)
            {
                msg.angles[i] = m_af_angle[i];
            }
        }

        {
            LOCK lock(m_mutex_m_af_speed);
            for (int i = 0; i < NB_JOINT; i++)
            {
                msg.vitesses[i] = m_af_speed[i];
            }
        }

        // This is stupid of me but I don't have time to update it. Cartesian
        // mode in GUI is 0 and joint is 1
        {
            LOCK lock(m_mutex_m_b_mode_cartesian);
            msg.ctrl_mode = static_cast<uint8_t>(!m_b_mode_cartesian);
        }

        {
            LOCK lock(m_mutex_m_n_current_joint);
            msg.current_joint = m_n_current_joint;
        }

        // Member not defined or used anymore
        msg.calibration = false;
        for (int i = 0; i < NB_JOINT; i++)
        {
            msg.enable[i] = true;
        }
        msg.kinetics_calc_error = false;
        msg.limiteur = false;
        msg.singular_matrix = false;
        msg.speed_multiplier = -69.0f;

        // Sending msg
        m_pub_feedback.publish(msg);
    }

    void CBHeartbeat(const std_msgs::EmptyConstPtr &msg)
    {
        LOCK lock(m_mutex_m_b_com_is_alive);
        m_b_com_is_alive = true;
    }

    void CBResetWatchdog(const ros::TimerEvent &event)
    {
        LOCK lock(m_mutex_m_b_com_is_alive);
        LOCK lock2(m_mutex_m_b_watchdog_is_alive);
        m_b_watchdog_is_alive = m_b_com_is_alive;
        m_b_com_is_alive = false;
    }
};

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "arm_master_node");
    ArmMasterNode arm_master_node;
    arm_master_node.Run();

    return 0;
}
