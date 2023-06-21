#include "ros/ros.h"
#include "rover_arm_msgs/watchdog.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "joint_watchdog");

    ros::NodeHandle n;

    ros::Publisher pub_watchdog = n.advertise<rover_arm_msgs::watchdog>(ros::this_node::getNamespace() + "/JWD", 1);
    ros::Rate timer(4);

    while(!ros::isShuttingDown())
    {
        rover_arm_msgs::watchdog msg;
        msg.connected = true;
        pub_watchdog.publish(msg);
        
        ros::spinOnce();
        timer.sleep();
    }
}
