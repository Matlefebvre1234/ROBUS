/*
Projet S1 2020
General librairies for Darth Genius Robus
@author Yan Ha Routhier-Chevrier, and the team
@version 1.0 08/11/2020
*/
#include <Arduino.h>
#include <LibRobus.h>
/**/
#ifndef RobusDarthGenius_H_
#define RobusDarthGenius_H_
// déclaration des variables
#define PI 3.1416
#define FRONT 0
#define TURN 1
#define SONAR 1
#define CAPTEUR 2

void reinitialiserVariable();
void LigneDroitePID2();
void Avancer(long pulse, bool detect);
void Virage_1roue(float angle);
void Virage_2roue(float angle);
long CmEnPulse (int distance_cm);
void reinitialiserVariable();
bool avertisseurSonore();
void RelacherBalle();
void AttraperBalle();
void testROBOT();
void Virage_1roueDroite(float angle);
#endif //RobusDarthGenius
