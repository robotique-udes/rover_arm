#include "moteur/Moteur.h"

void Moteur::setAngle(float iAngle) {angle = iAngle;}
float Moteur::getAngle() {return angle;}

void Moteur::setPeriod(float iPeriod) {period = iPeriod;}
float Moteur::getPeriod() {return period;}

void Moteur::setEnable(bool iEnable) {enable = iEnable;}
float Moteur::getEnable() {return enable;}

float Moteur::getDir() {return (period > 0) ? 1 : 0;}