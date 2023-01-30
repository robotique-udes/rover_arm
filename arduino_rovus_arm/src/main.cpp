#include <ros.h>
#include <rover_arm/vitesse_moteur_msg.h>
#include <rover_arm/angle.h>
#include "Servo.h"
#include "Stepper.h"
#include <string.h>

//__________________________________________________________________________________________
// Prototypes de fonctions :
void callback(const rover_arm::vitesse_moteur_msg &msg);
void stepMoteurs();
void sendMsg();
void armCalibration();

//__________________________________________________________________________________________
// Variables et constantes globales :
enum States
{
    standby,
    running,
    calibration
};
States state;

const int NB_STEPPER = 4;
const bool ENABLE = 1;
const bool DISABLE = 0;

bool flagCalib = 0;

// callback timer's var
const int CLOCK_CALLBACK = 10; // En millisecondes
unsigned long prev_millis_Callback;

// Creer les moteurs/joints
Stepper j1(21, 0, 30, 2, 69, 1600, 77, 1, 0);
Stepper j2(21, 0, 32, 3, 69, 1600, 150, 1, 0);
Stepper j3(21, 0, 34, 4, 69, 1600, 100, 1, 0);
Stepper j4(21, 0, 8, 9, 69, 1600, 100, 1, 0);

Stepper *steppers[4] = {&j1, &j2, &j3, &j4};

//__________________________________________________________________________________________
// ROS "setup/init" :
ros::NodeHandle n;
ros::Subscriber<rover_arm::vitesse_moteur_msg> sub("vitesses_moteur", callback);

rover_arm::angle angle;
ros::Publisher pub("valeurAngles", &angle);

//__________________________________________________________________________________________
void setup()
{
    //______________________________________________________________________________________
    // Ros :
    n.initNode();
    n.advertise(pub);
    n.subscribe(sub);

    // Set initial state
    state = running;
}

void loop()
{
    sendMsg();

    switch (state)
    {
    case standby:
        // nothing
        break;
    case running:
        stepMoteurs();
        break;

    case calibration:
        armCalibration();
        break;
    }
}

//__________________________________________________________________________________________
// Fonctions
void callback(const rover_arm::vitesse_moteur_msg &msg)
{
    j1.setPeriod(msg.Period[0]);
    j1.setDir(msg.Dir[0]);

    j2.setPeriod(msg.Period[1]);
    j2.setDir(msg.Dir[1]);

    j3.setPeriod(msg.Period[2]);
    j3.setDir(msg.Dir[2]);

    j4.setPeriod(msg.Period[3]);
    j4.setDir(msg.Dir[3]);

    // for (unsigned short i = 0; i < (sizeof(steppers) / sizeof(Stepper *)); i++)
    // {
    //     steppers[i]->setEnable(msg.En[i]);
    //     steppers[i]->setPeriod(msg.Period[i]);
    //     steppers[i]->setDir(msg.Dir[i]);
    // }
}

// Publie les valeurs des angles
void sendMsg()
{
    // Timer pour les messages (si trop rapide un delay exponentiel se cree)
    if (millis() - prev_millis_Callback > CLOCK_CALLBACK)
    {
        rover_arm::angle msg;
        // for (unsigned short i = 0; i < (sizeof(steppers) / sizeof(Stepper *)); i++)
        // {
        //     msg.angle[i] = steppers[i]->getAngle();
        // }

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
    for (unsigned short i = 0; i < (sizeof(steppers) / sizeof(Stepper *)); i++)
    {
        steppers[i]->doStep();
    }

    // j1.doStep();
    // j2.doStep();
    // j3.doStep();
    // j4.doStep();
}

void armCalibration()
{
    if (flagCalib)
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
