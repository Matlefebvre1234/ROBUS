#include <Arduino.h>
#include <LibRobus.h>
#include <RobusDarthGenius.h>
/////////////////////////////////////////////////////////////////////
//
// POUR le fonctionnement du robot A et B
// vitesse pour le robot B a .50
// vitesse pour le robot A a .70 - .75
// angle positive doit etre mise plus grande pour le robot B
// changement du PID
//
//  -cheers
//
/////////////////////////////////////////////////////////////////////
void setup()
{
    BoardInit();
    //AX_BuzzerON(5000);
    AX_BuzzerOFF();
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    Serial.begin(9600);
    reinitialiserVariable();
}
void prendreBallon()
{
    delay(1000);
    SERVO_Disable(LEFT);
    SERVO_Disable(RIGHT);
    RelacherBalle();
    Virage_2roue(93);
    delay(2000);
    reinitialiserVariable();
    Avancer(CmEnPulse(25));
    reinitialiserVariable();
    Virage_2roue(-90);
    reinitialiserVariable();
    Avancer(CmEnPulse(70));
    delay(1000); //couleur
    reinitialiserVariable();
    Avancer(CmEnPulse(150));
    reinitialiserVariable();
    AttraperBalle();
    delay(1000);

    if (0) //jaune
    {
        Avancer(CmEnPulse(84));
        reinitialiserVariable();
        Virage_2roue(93);
        reinitialiserVariable();
        Avancer(CmEnPulse(25));
        reinitialiserVariable();
    }
    else if (1) //bleu
    {
        Avancer(CmEnPulse(163));
        reinitialiserVariable();
        Virage_2roue(-90);
        reinitialiserVariable();
        Avancer(CmEnPulse(25));
        reinitialiserVariable();
    }
    else if (0) //rouge
    {
        Avancer(CmEnPulse(230));
        reinitialiserVariable();
        Virage_2roue(93);
        reinitialiserVariable();
        Avancer(CmEnPulse(25));
        reinitialiserVariable();
    }
    AttraperBalle();
}
void loop()
{   

    delay(1000);
    if (avertisseurSonore())
    {
        Serial.println("START");
        //prendreBallon();

        testROBOT();
        exit(0);
    }
}