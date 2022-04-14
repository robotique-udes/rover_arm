//test ROS serial and arduino implementation
//to launch : rosrun rosserial_python serial_node.py /dev/ttyACM0    or USB0
//to echo : rostopic echo chatter                               ^check for correct usb port

#include <ros.h>
#include <rovus_bras/vitesse_moteur_msg.h>
#include <rovus_bras/angle.h>


//__________________________________________________________________________________________
//Variables/constante globales :

//Moteur1
const int steps_m1 = 200;
const float gearboxRatio_m1 = 100.0;
const float stepPerDeg_m1 = steps_m1*gearboxRatio_m1/360;


//Pins :
int DIR_1 = 40;
int PUL_1 = 4;
int DIR_2 = 1;
int PUL_2 = 5;
int DIR_3 = 6;
int PUL_3 = 7;   

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


//__________________________________________________________________________________________
//Prototypes de fonctions :
void callback(const rovus_bras::vitesse_moteur_msg &msg);
void step_moteurs();


//__________________________________________________________________________________________
//ROS "init" :
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


    if (msg.m3 == 0)
    {
        period.m3 = 0;
    }
    else
    {
        period.m3 = 1000000/(abs(msg.m3)*stepPerDeg_m1);
    }
    dir.m3 = msg.m3;
}

void step_moteurs()
{  
    //Moteur #1
        if (dir.m3 > 0)
        {
            digitalWrite(DIR_3, HIGH);
        }
        else
        {
            digitalWrite(DIR_3, LOW);
        }

        
        char str[10];
        ultoa(period.m3, str, 10);
        n.loginfo(str);
        n.loginfo("o");
        
        
    
        if ((micros() - prev_micros.m3 > (period.m3)) && (period.m3 != 0))
        {
            n.loginfo("i");

            digitalWrite(PUL_3, HIGH);
            digitalWrite(PUL_3, LOW);
            prev_micros.m3 = micros();
        }
}