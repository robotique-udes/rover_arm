//test ROS serial and arduino implementation
//to launch : rosrun rosserial_python serial_node.py /dev/ttyACM0 or USB0
//to echo : rostopic echo chatter                               ^check for correct usb port

#include <ros.h>
#include <rovus_bras/vitesse_moteur_msg.h>
#include <rovus_bras/angle.h>
#include "Servo.h"
#include "Stepper.h"

//__________________________________________________________________________________________
//Prototypes de fonctions :
void callback(const rovus_bras::vitesse_moteur_msg &msg);
void stepMoteurs();
void sendMsg();
void armCalibration();

//__________________________________________________________________________________________
//Variables et constantes globales :
enum States {standby, running, calibration};
States state;
const int NB_STEPPER = 4;
bool flagCalib = 0;

//callback timer's var
const int CLOCK_CALLBACK = 10; //En millisecondes
unsigned long prev_millis_Callback;

//Creer les moteurs/joints
Stepper j1(21, 0, 30, 2, 69, 1600.0, 77.0, 1, 30.0);
Stepper j2(21, 0, 32, 3, 69, 1600.0, 150.0, 1, 20.0);
Stepper j3(21, 0, 34, 4, 69, 1600.0, 100.0, 1, 0.0);
Stepper j4(21, 0, 8, 9, 69, 1600.0, 100.0, 1, 0.0);

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

    //Set initial state
    state = running;
}

void loop()
{
    sendMsg();

    switch (state)
    {
        case standby:
            //All motor Disable
            break;
        
        case running:
            stepMoteurs();

        case calibration:
            armCalibration();
    }

    Stepper
}
//__________________________________________________________________________________________
//Fonctions
void callback(const rovus_bras::vitesse_moteur_msg &msg)
{
    j1.getPeriod(msg.Period[0]);
    j1.getDir(msg.Dir[0]);

    j2.getPeriod(msg.Period[1]);
    j2.getDir(msg.Dir[1]);

    j3.getPeriod(msg.Period[2]);
    j3.getDir(msg.Dir[2]);

    j4.getPeriod(msg.Period[3]);
    j4.getDir(msg.Dir[3]);
}

//Publie les valeurs des angles 
void sendMsg()
{
    //Timer pour les messages (si trop rapide un delay exponentiel se cree)
    if (millis() - prev_millis_Callback > CLOCK_CALLBACK)
    {
        rovus_bras::angle msg;
        msg.angle[0] = j1.getAngle();
        msg.angle[1] = j2.getAngle();
        msg.angle[2] = j3.getAngle();
        msg.angle[3] = j4.getAngle();
        pub.publish(&msg);
        n.spinOnce();
 

        prev_millis_Callback = millis();
    }
}

void stepMoteurs()
{    
    j1.doStep();
    j2.doStep();
    j3.doStep();
    j4.doStep();
}   

void armCalibration()
{
    if(flagCalib)
    {
        j1.setFlagCalib(1);
        j2.setFlagCalib(2);
        j3.setFlagCalib(3);
        j4.setFlagCalib(4);


        if (!j1.getFlagCalib())
            j1.calibration();

        else if (!j2.getFlagCalib())
            j2.calibration();

        else if (!j3.getFlagCalib())
            j3.calibration();

        else if (!j4.getFlagCalib())
            j4.calibration();
        else
        {
            flagCalib = 0;
            j1.setFlagCalib(0);
            j2.setFlagCalib(0);
            j3.setFlagCalib(0);
            j4.setFlagCalib(0);
        }
    }
}
