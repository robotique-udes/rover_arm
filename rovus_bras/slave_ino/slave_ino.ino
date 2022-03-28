//test ROS serial and arduino implementation

#include "ros.h"
#include <rovus_bras/vitesse_moteur_msg.h>
#include <rovus_bras/angle.h>
#include <stdio.h>
#include <string.h>

ros::NodeHandle  n;

ros::Subscriber sub = n.subscribe("vitesses_moteur", 5, callback);

ros::Publisher pub = n.advertise<rovus_bras::angle>("valeurAngles", 1000);
ros::Rate loop_rate(500);



void callback(const rovus_bras::vitesse_moteur_msg &msg)
{
    ROS_INFO("Received : \nVitesse M1:\t%f \nVitesse M2:\t%f \nVitesse M3:\t%f \nVitesse M4:\t%f \n\n", msg.m1, msg.m2, msg.m3, msg.m4);

}


void setup()
{
    
}

void loop();
