#include <Arduino.h>
#include <LibRobus.h>
#include <MusiqueRobus.h>
#include <RobusDarthGenius.h>
void setup()
{
    // BOILER PLATE SETUP
    BoardInit();
    Serial.begin(9600);
    reinitialiserVariable();
    // Allumer les PIN pour la detection de couleur
    pinMode(PinBLEU, OUTPUT);
    pinMode(PinJAUNE, OUTPUT);
    pinMode(PinROUGE, OUTPUT);
    pinMode(PinVERT, OUTPUT);
    // pin mode pour les suiveur de ligne
    pinMode(CPT_LIGNE_1, INPUT);
    pinMode(CPT_LIGNE_2, INPUT);
    pinMode(CPT_LIGNE_3, INPUT);
    pinMode(CPT_LIGNE_4, INPUT);
    pinMode(CPT_LIGNE_5, INPUT);
    pinMode(CPT_LIGNE_6, INPUT);
     pinMode(2,INPUT);
    attachInterrupt(0,buttonPress,RISING);

}

void prendreBallon()
{
    int couleur = 0;
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
    couleur = DetectionCouleur();
    reinitialiserVariable();
    Avancer(CmEnPulse(150));
    reinitialiserVariable();
    AttraperBalle();
    delay(500);

    if (couleur == JAUNE) //jaune
    {
        Avancer(CmEnPulse(84));
        reinitialiserVariable();
        Virage_2roue(93);
        reinitialiserVariable();
        Avancer(CmEnPulse(25));
        reinitialiserVariable();
    }
    else if (couleur == BLEU ) //bleu
    {
        Avancer(CmEnPulse(163));
        reinitialiserVariable();
        Virage_2roue(-90);
        reinitialiserVariable();
        Avancer(CmEnPulse(25));
        reinitialiserVariable();
    }
    else if (couleur == ROUGE) //rouge
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
    reinitialiserVariable();
    Avancer(CmEnPulse(45));
    reinitialiserVariable();
    Avancer(CmEnPulse(450));
    exit(0);
}