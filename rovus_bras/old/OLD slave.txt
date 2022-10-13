#include "ros/ros.h"
#include <rovus_bras/vitesse_moteur_msg.h>
#include <rovus_bras/angle.h>
#include <stdio.h>
#include <string.h>

void callback(const rovus_bras::vitesse_moteur_msg &msg)
{
    ROS_INFO("Received : \nVitesse M1:\t%f \nVitesse M2:\t%f \nVitesse M3:\t%f \nVitesse M4:\t%f \n\n", msg.m1, msg.m2, msg.m3, msg.m4);

}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "slave");

    ros::NodeHandle n;

    //Receive message
    ros::Subscriber sub = n.subscribe("vitesses_moteur", 5, callback);

    ros::Publisher pub = n.advertise<rovus_bras::angle>("valeurAngles", 1000);
    ros::Rate loop_rate(500);

    while (ros::ok())
    {
        //Créer et publier message
        rovus_bras::angle msg;
        msg.j1 = 35;
        msg.j2 = 40;
        msg.j3 = 45;
        msg.j4 = 50;
        ROS_INFO("Sending: \nJ1:\t%d \nJ2:\t%d \nJ3:\t%d \nJ4:\t%d \n\n", msg.j1, msg.j2, msg.j3, msg.j4); 
        pub.publish(msg);

        ros::spinOnce();

        loop_rate.sleep();
        
    }

    return 0;
}