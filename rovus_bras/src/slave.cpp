#include "ros/ros.h"
#include <rovus_bras/vitesse_moteur_msg.h>
#include <rovus_bras/angle.h>
#include <stdio.h>
#include <string.h>

void callback(const rovus_bras::vitesse_moteur_msg &msg)
{
    ROS_INFO("Vitesse M1 = %f", msg.m1);
    ROS_INFO("Vitesse M2 = %f", msg.m2);
    ROS_INFO("Vitesse M3 = %f", msg.m3);
    ROS_INFO("Vitesse M4 = %f\n\n", msg.m4);
}
 

int main(int argc, char **argv)
{

    ros::init(argc, argv, "slave");

    ros::NodeHandle n;

    //Receive message
    ros::Subscriber sub = n.subscribe("vitesses_moteur", 5, callback);

    ros::Publisher pub = n.advertise<rovus_bras::angle>("valeurAngles", 1000);
    ros::Rate loop_rate(1);

    while (ros::ok())
    {
        rovus_bras::angle msg;

        msg.j1 = 1;
        msg.j2 = 1;
        msg.j3 = 1;
        msg.j4 = 1;

        ROS_INFO("J1 = %d", msg.j1);
        ROS_INFO("J2 = %d", msg.j2);
        ROS_INFO("J3 = %d", msg.j3);
        ROS_INFO("J4 = %d\n\n", msg.j4);   
        pub.publish(msg);

        ros::spinOnce();

        loop_rate.sleep();
    }

    return 0;
}