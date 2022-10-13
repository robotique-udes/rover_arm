//test ROS serial and arduino implementation
//to launch : rosrun rosserial_python serial_node.py /dev/ttyUSB0
//to echo : rostopic echo chatter                               ^check for correct usb port

#include <ros.h>
#include <rovus_bras/vitesse_moteur_msg.h>
#include <rovus_bras/angle.h>
#include <stdio.h>
#include <string.h>

ros::NodeHandle n;

class vitesse_angulaire
{
    public:
        float m1;
        float m2;
        float m3;
        float m4;
} vitesses_recu;


void callback(const rovus_bras::vitesse_moteur_msg &msg)
{
    //log return --> Big impact on perf, use feedback from highlvl instead
    /*
    char m1[8], m2[8], m3[8], m4[8];
    n.loginfo("Message Received:\n");
    dtostrf(msg.m1,5,4,m1);
    dtostrf(msg.m2,5,4,m2);
    dtostrf(msg.m3,5,4,m3);
    dtostrf(msg.m4,5,4,m4);
    n.loginfo(m1);
    n.loginfo(m2);
    n.loginfo(m3);
    n.loginfo(m4);
    n.loginfo("\n\n");
    */

   vitesses_recu.m1 = msg.m1;
   vitesses_recu.m2 = msg.m2;
   vitesses_recu.m3 = msg.m3;
   vitesses_recu.m4 = msg.m4;
}

//Creating Pub and Sub
//ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );
ros::Subscriber<rovus_bras::vitesse_moteur_msg> sub("vitesses_moteur", callback);
rovus_bras::angle angle;
ros::Publisher pub("valeurAngles", &angle);


void setup()
{
    n.initNode();
    n.advertise(pub);
    n.subscribe(sub);

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    //Ecrire et Publier message --> Devrait être une fonction
    char m1[8], m2[8], m3[8], m4[8];
    dtostrf(vitesses_recu.m3,5,4,m3);
    n.loginfo(m3); 

    rovus_bras::angle msg;
    msg.j1 = 35;
    msg.j2 = 40;
    msg.j3 = 45;
    msg.j4 = 50;
    pub.publish(&msg);
    n.spinOnce();
}


//-------------------------------------------------------------------------------
