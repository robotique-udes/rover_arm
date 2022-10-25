#ifndef STEPPER
#define STEPPER

const bool DIR_POSITIF = 0;
const bool DIR_NEGATIF = 1;
const float VITESSE_CALIB = 5; //deg/s


class Stepper
{
    private:
        short SWITCH;
        short DIR;
        short PUL;
        float STEPS_REV;
        float GEARBOX;
        float STEPS_DEG;

        unsigned long prev_micros = 0;
        unsigned long period = 0;
        bool dir = 1;
        long curr_step = 0;
        short angle = 0;
        bool DIR_CALIB = 0;
        int STEP_CALIB = 8000;
        short SWITCH_STEP = 0;
        short state_calib = 0;
        bool flagCalib = 0;
        float Angle;

    public:
        Stepper(short iSwitchPin /*switch pin*/,
        int iSwitchAngle /*siwtch angle*/,
        short iDirPin /*DIR pin*/,
        short iPulPin /*PUL pin*/,
        float iStepPerDeg /*Step per rev*/,
        float iGearboxRation /*Gearbox ratio*/,
        bool  iDirCalib  /*Direction de la calibration (1 ou 0)*/,
        float   iAngleCalib /*Angle à atteindre après la calib*/);
        
        void getPeriod(float msg);
        void getDir(bool msg);
        void doStep();
        float getAngle() {return curr_step/STEPS_DEG;}
        void calibration();
        bool getdir_calib();

        bool getFlagCalib() {return flagCalib;}
        void setFlagCalib(bool value) {flagCalib = value;}

};

#endif