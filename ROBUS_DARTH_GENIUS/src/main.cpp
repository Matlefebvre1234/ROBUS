#include <Arduino.h>
#include <LibRobus.h>
#include <MusiqueRobus.h>
#include <RobusDarthGenius.h>
#include <SPI.h>
#include <MFRC522.h>
void setup()
{
    // BOILER PLATE SETUP
    BoardInit();
    Serial.begin(9600);
    SetRFID();
    reinitialiserVariable();
    // Allumer les PIN pour la detection de couleur
    // pin mode pour les suiveur de ligne
    pinMode(CPT_LIGNE_1, INPUT);
    pinMode(CPT_LIGNE_2, INPUT);
    pinMode(CPT_LIGNE_3, INPUT);
    pinMode(CPT_LIGNE_4, INPUT);
    pinMode(CPT_LIGNE_5, INPUT);
    pinMode(CPT_LIGNE_6, INPUT);
    pinMode(3,INPUT);
    attachInterrupt(1,buttonPress,RISING);
    MOTOR_SetSpeed(RIGHT,0);
    MOTOR_SetSpeed(LEFT,0);
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
    SetSteady(true);
    delay(1500);
    int doorCodeRFID = RFID();
    Serial.println(String(doorCodeRFID));
    if(doorCodeRFID == 1 ||doorCodeRFID == 2 ||doorCodeRFID == 3)
        SetSteady(false);
    while (!IsSteady())
    {
        bool humainDetecter = SONAR_GetRange(1) < 100;
        Serial.println(String(SONAR_GetRange(1)));
        if(SONAR_GetRange(1) < 100) 
        {
            humainDetecter =true;
        }
        else {
            delay(100);
            humainDetecter = (SONAR_GetRange(1) < 100);
        }
        if(CheckIntersection()) {
            MOTOR_SetSpeed(RIGHT,.1);
            MOTOR_SetSpeed(LEFT, .1);
            delay(1000);
            switch (doorCodeRFID)
            {
                case 1:
                    Virage_2roue(-90);
                    break;
                case 3:
                    Virage_2roue(90);
                    break;
            }           
        }
        else if(humainDetecter)
        {
            SuivreLigne();
        }
        else
        {   
            MOTOR_SetSpeed(RIGHT,0);
            MOTOR_SetSpeed(LEFT,0);
        }
    }
}