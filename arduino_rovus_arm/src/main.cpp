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
void calibration(class moteur *m);

//__________________________________________________________________________________________
//Variables/constantes globales :
const bool DIR_POSITIF = 0;
const bool DIR_NEGATIF = 1;
const float VITESSE_CALIB = 5; //deg/s
const int DEBOUNCE = 1000; //time in ms
bool calib = 0;
bool calib_mem = 0;

//callback timer's var
const int CLOCK_CALLBACK = 10; //En millisecondes
unsigned long prev_millis_Callback;

//Creer les moteurs/joints
class moteur 
{
  public:
        short SWITCH;
        short DIR;
        short PUL;
        float STEPS_REV;
        float GEARBOX;
        float STEPS_DEG;

        unsigned long prev_micros = 0;
        unsigned long period = 0;
        bool dir = 1;
        short curr_step = 0;
        short angle = 0;
        bool DIR_CALIB = 0;
        int STEP_CALIB = 8000;
        short SWITCH_STEP = 0;
        short state_calib = 0;
        bool calib_done = 0;

    moteur( 
            short v /*switch pin*/,
            int   c /*siwtch angle*/, 
            short w /*DIR pin*/, 
            short x /*PUL pin*/, 
            float y /*Step per rev*/, 
            float z /*Gearbox ratio*/, 
            bool  a  /*Direction de la calibration (1 ou 0)*/, 
            float   b /*Angle à atteindre après la calib*/
            )
    {
        SWITCH = v;
        SWITCH_STEP = c*STEPS_DEG;
        DIR = w;
        PUL = x;
        STEPS_REV = y;
        GEARBOX = z;
        STEP_CALIB = b;

        STEPS_DEG = STEPS_REV*GEARBOX/360.0;
        SWITCH_STEP = c*STEPS_DEG;
        STEP_CALIB = b*STEPS_DEG;

        pinMode(DIR, OUTPUT);
        pinMode(PUL, OUTPUT);
        pinMode(SWITCH, INPUT);
    }

    bool getdir_calib()
    {
        bool dir;

        if (curr_step > STEP_CALIB)
            dir = DIR_NEGATIF;

        if (curr_step < STEP_CALIB)
            dir = DIR_POSITIF;

        return dir;
    }

};

//J1
moteur m1(21, 0, 40, 4, 1600.0, 100.0, 1, 30.0);
moteur m2(20, 0, 1, 5, 1600.0, 100.0, 1, 20.0);
moteur m3(20, 0, 6, 7, 1600.0, 100.0, 1, 0.0);
moteur m4(20, 0, 8, 9, 1600.0, 100.0, 1, 0.0);


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

    if (calib)
    {
        if (!m1.calib_done)
            calibration(&m1);

        else if (!m2.calib_done)
            calibration(&m2);

        else if (!m3.calib_done)
            calibration(&m3);

        else if (!m4.calib_done)
            calibration(&m4);
        else
        {
            calib = 0;
            m1.calib_done = 0;
            m2.calib_done = 0;
            m3.calib_done = 0;
            m4.calib_done = 0;
        }
    }
    else
        step_moteurs();

}



//__________________________________________________________________________________________
void callback(const rovus_bras::vitesse_moteur_msg &msg)
{
    if (!calib)
    {
        getPeriod(msg.m1, &m1);
        getDir(msg.m1, &m1);

        getPeriod(msg.m2, &m2);
        getDir(msg.m2, &m2);

        getPeriod(msg.m3, &m3);
        getDir(msg.m3, &m3);

        getPeriod(msg.m4, &m4);
        getDir(msg.m4, &m4);

        calib = msg.calib;
    }
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
        m->period = 1000000/((abs(msg) * m->STEPS_DEG));
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

void calibration(class moteur *m)
{
    /*
    Fonctionne comme une state machine 
    (pour permettre la transmission de message pendant l'execution)
    */
    switch (m->state_calib)
    {
        case 0: //dans le cas que ça rentre dans la fonction
            m->state_calib = 1;
            break;
        
        case 1: //initialisation de la calib
            m->dir = m->getdir_calib();
            m->period = (1000000/(VITESSE_CALIB*m->STEPS_DEG));

            m->state_calib = 2;
            break;

        case 2: //Faire des steps jusqu'à la switch
            doStep(m);
            if (digitalRead(m->SWITCH) == 1)
            {
                m->state_calib = 3;
            }
            break;

        case 3:
            m->curr_step = m->SWITCH_STEP;
        
            if (m->curr_step > m->STEP_CALIB)
                m->dir = DIR_NEGATIF;
            else if (m->curr_step < m->STEP_CALIB)
                m->dir = DIR_POSITIF; 

            m->state_calib = 4;

            break;

        case 4:
            if(m->curr_step == m->STEP_CALIB)
            {
                m->state_calib = 0;
                m->calib_done = 1;
            }
            doStep(m);  
            break;
    }

    return;
    
}