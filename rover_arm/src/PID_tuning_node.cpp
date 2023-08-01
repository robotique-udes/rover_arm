#include "ros/ros.h"
#include "rover_arm_msgs/joint_state.h"
#include "rover_arm_msgs/PID.h"
#include "rosbag/bag.h"
#include "rosbag/view.h"

void CBJS(const rover_arm_msgs::joint_stateConstPtr &msg);
void CBDJS(const rover_arm_msgs::joint_stateConstPtr &msg);

struct Speeds
{
    float target = 0.0f;
    float real = 0.0f;
} speeds;

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "PID_tuning_node");
    ros::NodeHandle n;

    ros::Subscriber sub_JS = n.subscribe<rover_arm_msgs::joint_state>("/arm/j1/JS", 1, CBJS);
    ros::Subscriber sub_DJS = n.subscribe<rover_arm_msgs::joint_state>("/arm/j1/DJS", 1, CBDJS);
    ros::Publisher  pub_PID = n.advertise<rover_arm_msgs::PID>("/arm/j1/PID", 1);

    float results[20][2] = {0};
    float KI = 0;
    for(int i = 0; i < 10; i++)
    {
        float KI = static_cast<float>(140 + i*10);

        rover_arm_msgs::PID pid_coeff;
        pid_coeff.kP = 15.0f;
        pid_coeff.kI = KI;
        pid_coeff.kD = 0.0f;
        pub_PID.publish(pid_coeff);

        ROS_INFO("Testing KI: %f", KI);
        results[i][0] = KI;

        ros::Time start_time = ros::Time::now();
        system("rosbag play testPID.bag& 2>&-");
        for(; (ros::Time::now() - start_time) < ros::Duration(25.0);)
        {
            ros::spinOnce();
            results[i][1] += abs(speeds.target - speeds.real)/100000;
        }

        ROS_INFO("Cummulative diff for KI: %f = %f", results[i][0], results[i][1]);

        KI += 50;
        ros::spinOnce();
    }

    return 0;
}

void CBJS(const rover_arm_msgs::joint_stateConstPtr &msg)
{
    speeds.real = msg->speed;
}

void CBDJS(const rover_arm_msgs::joint_stateConstPtr &msg)
{
    speeds.target = msg->speed;
}
