//test ROS serial and arduino implementation
//to launch : rosrun rosserial_python serial_node.py /dev/ttyUSB0
//to echo : rostopic echo chatter                               ^check for correct usb port

#define TIMER_INTERRUPT_DEBUG       0
#define TIMERINTERRUPT_LOGLEVEL     0
#define USE_TIMER_3     true
#define USE_TIMER_4     true
#define USE_TIMER_5     true

#include <ros.h>
#include <rovus_bras/vitesse_moteur_msg.h>
#include <rovus_bras/angle.h>
#include <stdio.h>
#include <string.h>
#include "TimerInterrupt.h"

//------------------------------------------------------------------------------------------

int DIR_1 = 40;
int PUL_1 = 4;
//int DIR_2 = 1;
//int PUL_2 = 5;
//int DIR_3 = 6;
//int PUL_3 = 7;   

bool PUL1_STATE = LOW;
//bool PUL2_STATE = LOW;
//bool PUL3_STATE = LOW;

double Step_ctr = 0;
void stepper1(float);
//------------------------------------------------------------------------------------------
inline void callback1();


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

   /*
   vitesses_recu.m1 = msg.m1;
   vitesses_recu.m2 = msg.m2;
   vitesses_recu.m3 = msg.m3;
   vitesses_recu.m4 = msg.m4;
   */

    float STEPS = 200.0;
    float GearBoxRation = 100.0; 
    float step_per_deg = STEPS*GearBoxRation/360.0;
    float SPEED_STEP_SEC = msg.m3 * step_per_deg;
    stepper1(SPEED_STEP_SEC);
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

//-------------------------------------------------------------
    pinMode(DIR_1, OUTPUT);
    pinMode(PUL_1, OUTPUT);
    //pinMode(PUL_2, OUTPUT);
    //pinMode(PUL_3, OUTPUT);

    ITimer3.init();
    //ITimer4.init();
    //ITimer5.init();
    //Serial.println("Setup");
}

void loop()
{
    //Ecrire et Publier message --> Devrait être une fonction

    rovus_bras::angle msg;
    msg.j1 = 35;
    msg.j2 = 40;
    msg.j3 = 45;
    msg.j4 = 50;
    pub.publish(&msg);
    n.spinOnce();
    
}


//-------------------------------------------------------------------------------
inline void callback1()
{
  digitalWrite(PUL_1, PUL1_STATE);
  PUL1_STATE = !PUL1_STATE;
  //Serial.println("callback");
}

void stepper1(float v1)
{
  //v1 = classe::vmot.m1

    if (v1 != 0){

        if (v1 > 0){
        digitalWrite(DIR_1,HIGH);     
        }
        else{
        digitalWrite(DIR_1,LOW);
        }

    ITimer3.attachInterrupt(abs(v1)*2, callback1 );
    
 
  }
  else
    ITimer3.detachInterrupt();

}