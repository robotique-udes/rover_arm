#pragma once

// Classes
class Moteur
{
    private:
        float angle = 0.0;
        float period = 0.0;
        bool enable = true;
        bool dir = 0;

    public:
        void setAngle(float iAngle);
        float getAngle();

        void setPeriod(float iPeriod);
        float getPeriod();

        void setEnable(bool iEnable);
        float getEnable();

        float getDir();
};
