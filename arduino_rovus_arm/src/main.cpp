//test ROS serial and arduino implementation
//to launch : rosrun rosserial_python serial_node.py /dev/ttyACM0 or USB0
//to echo : rostopic echo chatter                               ^check for correct usb port

#include <ros.h>
#include <rovus_bras/vitesse_moteur_msg.h>
#include <rovus_bras/angle.h>

//__________________________________________________________________________________________
//Variables/constante globales :

//J1
const short DIR_M1 = 40;
const short PUL_M1 = 4;
const int STEPS_M1 = 200;
const float GEARBOX_RATIO_M1 = 100.0;
const float STEPS_PER_DEG_M1 = STEPS_M1*GEARBOX_RATIO_M1/360;

//J2
const short DIR_M2 = 1;
const short PUL_M2 = 5;
const int STEPS_M2 = 200;
const float GEARBOX_RATIO_M2 = 100.0;
const float STEPS_PER_DEG_M2 = STEPS_M2*GEARBOX_RATIO_M2/360;

//J3
const short DIR_M3 = 6;
const short PUL_M3 = 6;
const int STEPS_M3 = 200;
const float GEARBOX_RATIO_M3 = 100.0;
const float STEPS_PER_DEG_M3 = STEPS_M3*GEARBOX_RATIO_M3/360;

//J4
const short DIR_M4 = 7;
const short PUL_M4 = 5;
const int STEPS_M4 = 200;
const float GEARBOX_RATIO_M4 = 100.0;
const float STEPS_PER_DEG_M4 = STEPS_M3*GEARBOX_RATIO_M3/360;

//Logic variables
struct memoire_dernier_step
{
    unsigned long m1 = 0;
    unsigned long m2 = 0;
    unsigned long m3 = 0;
    unsigned long m4 = 0;

} prev_micros;

struct period_moteur
{
    unsigned long m1 = 0;
    unsigned long m2 = 0;
    unsigned long m3 = 0;
    unsigned long m4 = 0;

} period;

struct direction_moteur
{
    int m1 = 0;
    int m2 = 0;
    int m3 = 0;
    int m4 = 0;

} dir;

unsigned long prev_spin_millis;


//__________________________________________________________________________________________
//Prototypes de fonctions :
void callback(const rovus_bras::vitesse_moteur_msg &msg);
void step_moteurs();
unsigned long getPeriod(float msg, float step_per_deg);
void DoStep(char nom_moteur[2], int dir, const int DIR_PIN, int PUL_PIN,
            unsigned long *prev_micros, unsigned long period);
            
//__________________________________________________________________________________________
//ROS "setup/init" :
ros::NodeHandle n;
ros::Subscriber<rovus_bras::vitesse_moteur_msg> sub("vitesses_moteur", callback);
rovus_bras::angle angle;
ros::Publisher pub("valeurAngles", &angle);

//__________________________________________________________________________________________

void setup()
{
    //______________________________________________________________________________________
    //Ros :
    n.initNode();
    n.advertise(pub);
    n.subscribe(sub);

    //______________________________________________________________________________________
    //Set pinMode
    pinMode(DIR_M1, OUTPUT);
    pinMode(PUL_M1, OUTPUT);

    pinMode(DIR_M2, OUTPUT);
    pinMode(PUL_M2, OUTPUT);
    
    pinMode(DIR_M3, OUTPUT);
    pinMode(PUL_M3, OUTPUT);    
    
    pinMode(DIR_M4, OUTPUT);
    pinMode(PUL_M4, OUTPUT);
}

void loop()
{
    //Ecrire et Publier message --> Devrait être une fonction

    //Timer pour les messages (si trop rapide un delay exponentiel se cree)
    if (millis() - prev_spin_millis > 10)
    {
        rovus_bras::angle msg;
        msg.j1 = 35;
        msg.j2 = 40;
        msg.j3 = 45;
        msg.j4 = 50; 
        pub.publish(&msg);

        n.spinOnce();
        prev_spin_millis = millis();
    }

    step_moteurs();
}


//__________________________________________________________________________________________
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

    period.m1 = getPeriod(msg.m1, STEPS_PER_DEG_M1);
    period.m2 = getPeriod(msg.m2, STEPS_PER_DEG_M2);           
    period.m3 = getPeriod(msg.m3, STEPS_PER_DEG_M3);
    period.m4 = getPeriod(msg.m4, STEPS_PER_DEG_M4);
}

void step_moteurs()
{    
    
    DoStep("m1", dir.m1, DIR_M1, PUL_M1, &prev_micros.m1, period.m1);
    DoStep("m2", dir.m2, DIR_M2, PUL_M2, &prev_micros.m2, period.m2);
    DoStep("m3", dir.m3, DIR_M3, PUL_M3, &prev_micros.m3, period.m3);
    DoStep("m4", dir.m4, DIR_M4, PUL_M4, &prev_micros.m4, period.m4);

}   

unsigned long getPeriod(float msg, float step_per_deg)
{
    unsigned long period = 0;

    if (msg == 0)
        period = 0;
    else
        period = 1000000/(abs(msg)*STEPS_PER_DEG_M3);

    return period;
}

void DoStep(char nom_moteur[2], int dir, const int DIR_PIN, int PUL_PIN,
            unsigned long *prev_micros, unsigned long period)
{
    if (dir > 0)
        digitalWrite(DIR_PIN, HIGH);
    else
        digitalWrite(DIR_PIN, LOW);

    if ((micros() - *prev_micros > (period)) && (period != 0))
    {
        n.loginfo(nom_moteur);

        digitalWrite(PUL_M3, HIGH);
        digitalWrite(PUL_M3, LOW);
        *prev_micros = micros();
    }
}











