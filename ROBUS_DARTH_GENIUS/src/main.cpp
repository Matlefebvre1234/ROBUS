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
    Avancer(CmEnPulse(25), false);
    reinitialiserVariable();
    Virage_2roue(-90);
    reinitialiserVariable();
    Avancer(CmEnPulse(70), false);
    delay(1000); //couleur
    couleur = DetectionCouleur();
    reinitialiserVariable();
    Avancer(CmEnPulse(150), false);
    reinitialiserVariable();
    AttraperBalle();
    delay(500);

    if (couleur == JAUNE) //jaune
    {
        Avancer(CmEnPulse(84), false);
        reinitialiserVariable();
        Virage_2roue(93);
        reinitialiserVariable();
        Avancer(CmEnPulse(25), false);
        reinitialiserVariable();
    }
    else if (couleur == BLEU ) //bleu
    {
        Avancer(CmEnPulse(163), false);
        reinitialiserVariable();
        Virage_2roue(-90);
        reinitialiserVariable();
        Avancer(CmEnPulse(25), false);
        reinitialiserVariable();
    }
    else if (couleur == ROUGE) //rouge
    {
        Avancer(CmEnPulse(230), false);
        reinitialiserVariable();
        Virage_2roue(93);
        reinitialiserVariable();
        Avancer(CmEnPulse(25), false);
        reinitialiserVariable();
    }
    AttraperBalle();
}

void loop()
{
    delay(1000);
    reinitialiserVariable();
    //prendreBallon();
    float distanceSonar = 0;
    distanceSonar = SONAR_GetRange(1);
    delay(500);
    Serial.println(String(distanceSonar));
    //testROBOT();
    //distanceSonar = SONAR_GetRange(1);
    int detected = 0;
    while (detected < 2)
    {
        if(!avertisseurSonore())
            detected = 0;
        else
            detected += 1;
        delay(500);
    }
    Serial.println("Start");
    
    Avancer(CmEnPulse(40), false);
    Avancer(CmEnPulse(420), true);
    // delay(500);
    sensualSong();
    //sensualSong();
    exit(0);
}