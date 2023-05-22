#include "ros/ros.h"
#include "rover_arm_msg/watchdog.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "joint_watchdog");

    ros::NodeHandle n;

    ros::Publisher pub_watchdog = n.advertise<rover_arm_msg::watchdog>("/JWD", 1);
    ros::Rate timer(4);

    while(!ros::isShuttingDown())
    {
        rover_arm_msg::watchdog msg;
        msg.connected = true;
        pub_watchdog.publish(msg);
        
        ros::spinOnce();
        timer.sleep();
    }
}