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
#define PLANCHER 0
#define JAUNE 1
#define BLEU 2
#define ROUGE 3
#define VERT 4
#define PinJAUNE 8
#define PinBLEU 9
#define PinROUGE 11
#define PinVERT 13
#define CPT_LIGNE_1 2
#define CPT_LIGNE_2 3
#define CPT_LIGNE_3 4
#define CPT_LIGNE_4 5
#define CPT_LIGNE_5 6
#define CPT_LIGNE_6 7


void reinitialiserVariable();
void LigneDroitePID2();
void Avancer(long pulse);
void Virage_1roue(float angle);
void Virage_2roue(float angle);
long CmEnPulse (int distance_cm);
void reinitialiserVariable();
bool avertisseurSonore();
void RelacherBalle();
void AttraperBalle();
void testROBOT();
int DetectionCouleur();
void Virage_1roueDroite(float angle);
#endif //RobusDarthGenius
