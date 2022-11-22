#include "Stepper.h"
#include "ros.h"

Stepper::Stepper(short iSwitchPin /*switch pin*/,
        int iSwitchAngle /*siwtch angle*/,
        short iDirPin /*DIR pin*/,
        short iPulPin /*PUL pin*/,
        short iEnPin /*Enable pin*/,
        float iStepPerDeg /*Step per rev*/,
        float iGearboxRation /*Gearbox ratio*/,
        bool  iDirCalib  /*Direction de la calibration (1 ou 0)*/,
        float   iAngleCalib /*Angle à atteindre après la calib*/)
{
        SWITCH = iSwitchPin;
        SWITCH_STEP = iSwitchAngle*STEPS_DEG;
        DIR = iDirPin;
        PUL = iPulPin;
        EN = iEnPin;
        STEPS_REV = iStepPerDeg;
        GEARBOX = iGearboxRation;
        STEP_CALIB = iAngleCalib;

        STEPS_DEG = STEPS_REV*GEARBOX/360.0;
        SWITCH_STEP = iSwitchAngle*STEPS_DEG;
        STEP_CALIB = iAngleCalib*STEPS_DEG;

        pinMode(DIR, OUTPUT);
        pinMode(PUL, OUTPUT);
        pinMode(SWITCH, INPUT);
        pinMode(EN, OUTPUT);
}


void Stepper::getPeriod(float msg)
{
    if (msg == 0)
        period = 0;
    else
        period = 1000000/((abs(msg) * STEPS_DEG));
}

void Stepper::getDir(bool msg)
{
    dir = msg;
}

void Stepper::doStep()
{
    dir? digitalWrite(DIR, HIGH): digitalWrite(DIR, LOW);

    if ((micros() - prev_micros > (period)) && (period != 0))
    {
        dir? curr_step += 1: curr_step -= 1;

        prev_micros = micros();
        digitalWrite(PUL, HIGH);
        digitalWrite(PUL, LOW);	
	}
}

void Stepper::calibration()
{
    /*
    Fonctionne comme une state machine 
    (pour permettre la transmission de message pendant l'execution)
    */
    switch (state_calib)
    {
        case 0: //dans le cas que ça rentre dans la fonction
            state_calib = 1;
            break;
        
        case 1: //initialisation de la calib
            dir = getdir_calib();
            period = (1000000/(VITESSE_CALIB*STEPS_DEG));

            state_calib = 2;
            break;

        case 2: //Faire des steps jusqu'à la switch
            doStep();
            if (digitalRead(SWITCH) == 1)
            {
                state_calib = 3;
            }
            break;

        case 3:
            curr_step = SWITCH_STEP;
        
            if (curr_step > STEP_CALIB)
                dir = DIR_NEGATIF;
            else if (curr_step < STEP_CALIB)
                dir = DIR_POSITIF; 

            state_calib = 4;

            break;

        case 4:
            if(curr_step == STEP_CALIB)
            {
                state_calib = 0;
                flagCalib = 1;
            }
            doStep();  
            break;
    }

    return;   	
}

bool Stepper::getdir_calib()
{
	bool dir;

	if (curr_step > STEP_CALIB)
			dir = DIR_NEGATIF;

	if (curr_step < STEP_CALIB)
			dir = DIR_POSITIF;

	return dir;
}

void Stepper::setEnable(bool enState) 
{
    digitalWrite(EN, enState);
}
