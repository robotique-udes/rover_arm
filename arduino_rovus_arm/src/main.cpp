//test ROS serial and arduino implementation
//to launch : rosrun rosserial_python serial_node.py /dev/ttyACM0 or USB0
//to echo : rostopic echo chatter                               ^check for correct usb port

#include <ros.h>
#include <rovus_bras/vitesse_moteur_msg.h>
#include <rovus_bras/angle.h>

//__________________________________________________________________________________________
//Prototypes de fonctions :
void callback(const rovus_bras::vitesse_moteur_msg &msg);
void step_moteurs();
void getPeriod(float msg, class moteur *m);
void getDir(float msg, class moteur *m);
void doStep(class moteur *m);
float getAngle(class moteur *m);

//__________________________________________________________________________________________
//Variables/constante globales :

//callback timer's var
const int CLOCK_CALLBACK = 10; //En millisecondes
unsigned long prev_millis_Callback;

//Creer les moteurs/joints
class moteur 
{
  public:
        short DIR;
        short PUL;
        short STEPS_REV;
        short GEARBOX;
        float STEPS_DEG;

        unsigned long prev_micros = 0;
        unsigned long period = 0;
        bool dir = 1;
        short curr_step = 0;
        short angle = 0;

    moteur(short w, short x, short y, short z)
    {
        DIR = w;
        PUL = x;
        STEPS_REV = y;
        GEARBOX = z;

        STEPS_DEG = STEPS_REV*GEARBOX/360;

        pinMode(DIR, OUTPUT);
        pinMode(PUL, OUTPUT);
    }
};

//J1
moteur m1(40, 4, 200, 100);
moteur m2(1, 5, 200, 100);
moteur m3(6, 7, 200, 100);
moteur m4(8, 9, 200, 100);


//__________________________________________________________________________________________
//ROS "setup/init" :
ros::NodeHandle n;
ros::Subscriber<rovus_bras::vitesse_moteur_msg> sub("vitesses_moteur", callback);

//rovus_bras::angle angle;
//ros::Publisher pub("valeurAngles", &angle);

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
}

void loop()
{
    //Ecrire et Publier message --> Devrait être une fonction

    //Timer pour les messages (si trop rapide un delay exponentiel se cree)
    if (millis() - prev_millis_Callback > CLOCK_CALLBACK)
    {
        rovus_bras::angle msg;
        msg.j1 = getAngle(&m1);
        msg.j2 = getAngle(&m2);
        msg.j3 = getAngle(&m3);
        msg.j4 = getAngle(&m4); 
        pub.publish(&msg);
        n.spinOnce();

        prev_millis_Callback = millis();
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

    getPeriod(msg.m1, &m1);
    getDir(msg.m1, &m1);

    getPeriod(msg.m2, &m2);
    getDir(msg.m2, &m2);

    getPeriod(msg.m3, &m3);
    getDir(msg.m3, &m3);

    getPeriod(msg.m4, &m4);
    getDir(msg.m4, &m4);
}

void step_moteurs()
{    
    doStep(&m1);
    doStep(&m2);
    doStep(&m3);
    doStep(&m4);
}   

void getPeriod(float msg, class moteur *m)
{
    if (msg == 0)
        m->period = 0;
    else
        m->period = 1000000/(abs(msg)*m->STEPS_DEG);
}

void getDir(float msg, class moteur *m)
{
    if (msg > 0)
        m->dir = 1;
    else
        m->dir = 0;
}

void doStep(class moteur *m)
{
    if (m->dir)
        digitalWrite(m->DIR, HIGH);
    else
        digitalWrite(m->DIR, LOW);

    if ((micros() - m->prev_micros > (m->period)) && (m->period != 0))
    {
        if (m->dir)
            m->curr_step += 1;
        else
            m->curr_step -= 1;
        
        m->prev_micros = micros();
        digitalWrite(m->PUL, HIGH);
        digitalWrite(m->PUL, LOW);
    }    
}

float getAngle(class moteur *m)
{
    return (m->curr_step/m->STEPS_DEG);
}