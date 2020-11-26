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
#define TURN 1
#define SONAR 1
#define CAPTEUR 2
#define JAUNE 1
#define BLEU 2
#define ROUGE 3
#define VERT 4
#define PinJAUNE 8
#define PinBLEU 9
#define PinROUGE 11
#define PinVERT 13
#define Puce1 "3C B5 FC 22"
#define Puce2 "5C 8C 24 22"
#define Puce3 "4C D1 B2 21"
#define Puce4 "07 83 D2 C6"
#define Carte1 "49 2B 25 BD"
#define Carte2 "79 89 60 B2"
#define Carte3 "09 C8 55 B3"
#define Carte4 "49 F9 71 B2"

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
int DetectionCouleur();
void Virage_1roueDroite(float angle);
#endif //RobusDarthGenius
