//test ROS serial and arduino implementation
//to launch : rosrun rosserial_python serial_node.py /dev/ttyACM0    or USB0
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

int DIR_3 = 40;
int PUL_3 = 4;
//int DIR_2 = 1;
//int PUL_2 = 5;
//int DIR_3 = 6;
//int PUL_3 = 7;   

bool PUL1_STATE = LOW;
//bool PUL2_STATE = LOW;
//bool PUL3_STATE = LOW;

double Step_ctr = 0;
void stepper1(float);

unsigned long m1_prev_micros = 0;
unsigned long m2_prev_micros = 0;
unsigned long m3_prev_micros = 0;
unsigned long m4_prev_micros = 0;

int dir_m1 = 0;
int dir_m2 = 0;    
int dir_m3 = 0;
int dir_m4 = 0;

//------------------------------------------------------------------------------------------
void step_moteurs();

ros::NodeHandle n;

struct period_moteur
{
    unsigned long m1;
    unsigned long m2;
    unsigned long m3;
    unsigned long m4;
} period;

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
    float temp;

    if (msg.m3 == 0)
    {
        period.m3 = 2000000;
    }
    else
    {
        temp = 1000000/(abs(msg.m3)*step_per_deg);
        period.m3 = 1000000/(abs(msg.m3)*step_per_deg);

        char period_m3_str[10];
        ultoa(period.m3, period_m3_str,10);
        n.loginfo(period_m3_str);

    }
    dir_m3 = msg.m3;
}

//Creating Pub and Sub
//ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );
ros::Subscriber<rovus_bras::vitesse_moteur_msg> sub("vitesses_moteur", callback);
rovus_bras::angle angle;
ros::Publisher pub("valeurAngles", &angle);


void setup()
{
    n.initNode();
    // n.setSpinTimeout(10);
    n.advertise(pub);
    n.subscribe(sub);
    m3_prev_micros = micros();

//-------------------------------------------------------------
    pinMode(DIR_3, OUTPUT);
    pinMode(PUL_3, OUTPUT);
    //pinMode(PUL_2, OUTPUT);
    //pinMode(PUL_3, OUTPUT);

    // ITimer3.init();
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

    //n.loginfo(char(period.m3));

    step_moteurs();
    
}


//-------------------------------------------------------------------------------

void step_moteurs()
{  
    //Moteur #1
    if (dir_m3 >0)
    {
        digitalWrite(DIR_3, HIGH);
    }
    else
    {
        digitalWrite(DIR_3, LOW);
    }

    n.loginfo("o");

    // char period_m3_str[10];
    // ultoa(period.m3, period_m3_str,10);
    // n.loginfo(period_m3_str);

    if ((micros() - m3_prev_micros > (period.m3))) //&& (period.m3 != 0)))
    {
        n.loginfo("i");
        digitalWrite(PUL_3, HIGH);
        digitalWrite(PUL_3, LOW);
        m3_prev_micros = micros();
        //n.loginfo("Je suis dans un step");
    }
}