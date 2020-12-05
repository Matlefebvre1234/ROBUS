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
    reinitialiserVariable();
    // Allumer les PIN pour la detection de couleur
    // pin mode pour les suiveur de ligne
    pinMode(CPT_LIGNE_1, INPUT);
    pinMode(CPT_LIGNE_2, INPUT);
    pinMode(CPT_LIGNE_3, INPUT);
    pinMode(CPT_LIGNE_4, INPUT);
    pinMode(CPT_LIGNE_5, INPUT);
    pinMode(CPT_LIGNE_6, INPUT);
    pinMode(19,INPUT);
    attachInterrupt(4,buttonPress,RISING);
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
    int doorCodeRFID;
    SetSteady(true);
    delay(1500);
      Serial.println("Avant");
     Serial.println(String(doorCodeRFID));
    SetRFID();
     doorCodeRFID = RFID();
     Serial.println("apres");
    Serial.println(String(doorCodeRFID));
    if(doorCodeRFID == 1 ||doorCodeRFID == 2 ||doorCodeRFID == 3)
        SetSteady(false);
    bool humainDetecter;
    bool humain;
    bool intersection;
    int nBintersection =0;
    while (!IsSteady())
    {
        intersection = false;
            for(int i =0;i<3;i++)
            {
                if(CheckIntersection()){ 
                    intersection = true;
                }

            }
            
        if(intersection) {
            nBintersection ++;
           
            if(nBintersection == 2)
            {
                MOTOR_SetSpeed(RIGHT,0);
                 MOTOR_SetSpeed(LEFT,0);
                 delay(1000);
                  MOTOR_SetSpeed(RIGHT,-0.1);
                 MOTOR_SetSpeed(LEFT,-0.1);
                 delay(1000);
                 MOTOR_SetSpeed(RIGHT, -.1);
                    MOTOR_SetSpeed(LEFT, .1);
                    delay(2550*2);


                 
            }
            else if(nBintersection == 1)
            {
                 MOTOR_SetSpeed(RIGHT,0);
            MOTOR_SetSpeed(LEFT,0);
            delay(2000);
            MOTOR_SetSpeed(RIGHT, .1);
            MOTOR_SetSpeed(LEFT, .1);
            delay(1000);
                switch (doorCodeRFID)
                {
                case 1:
                    MOTOR_SetSpeed(RIGHT, -.1);
                    MOTOR_SetSpeed(LEFT, .1);
                    delay(2550);
                    MOTOR_SetSpeed(RIGHT, 0.1);
                    MOTOR_SetSpeed(LEFT, 0.1);
                    delay(1000);
                    break;
                case 3:
                     MOTOR_SetSpeed(RIGHT, .1);
                    MOTOR_SetSpeed(LEFT, -.1);
                    delay(2550);
                   MOTOR_SetSpeed(RIGHT, 0.1);
                    MOTOR_SetSpeed(LEFT, 0.1);
                    delay(1000);
                    break;
                case 2:
                    MOTOR_SetSpeed(RIGHT, .1);
                    MOTOR_SetSpeed(LEFT, .1);
                    delay(1000);
                    MOTOR_SetSpeed(RIGHT, 0);
                    MOTOR_SetSpeed(LEFT, 0);
                 }           



            }
             else if( nBintersection == 3)
            {
                 MOTOR_SetSpeed(RIGHT,0);
            MOTOR_SetSpeed(LEFT,0);
            delay(2000);
            MOTOR_SetSpeed(RIGHT, .1);
            MOTOR_SetSpeed(LEFT, .1);
            delay(1000);
                switch (doorCodeRFID)
                {
                case 3:
                    MOTOR_SetSpeed(RIGHT, -.1);
                    MOTOR_SetSpeed(LEFT, .1);
                    delay(2550);
                  MOTOR_SetSpeed(RIGHT, 0.1);
                    MOTOR_SetSpeed(LEFT, 0.1);
                    delay(1000);
                    break;
                case 1:
                     MOTOR_SetSpeed(RIGHT, .1);
                    MOTOR_SetSpeed(LEFT, -.1);
                    delay(2550);
                    MOTOR_SetSpeed(RIGHT, 0.1);
                    MOTOR_SetSpeed(LEFT, 0.1);
                    delay(1000);
                    break;
                case 2:
                    MOTOR_SetSpeed(RIGHT, .1);
                    MOTOR_SetSpeed(LEFT, .1);
                    delay(1000);
                    MOTOR_SetSpeed(RIGHT, 0);
                    MOTOR_SetSpeed(LEFT, 0);
                 }           



            }
        else if(nBintersection == 4)
        {
            MOTOR_SetSpeed(RIGHT, 0);
            MOTOR_SetSpeed(LEFT, 0);
            delay(1000);
            MOTOR_SetSpeed(RIGHT, -.1);
            MOTOR_SetSpeed(LEFT, .1);
            delay(5100);
            MOTOR_SetSpeed(RIGHT, 0);
            MOTOR_SetSpeed(LEFT, 0);
            nBintersection =0;
            SetSteady(true);
            doorCodeRFID = 0;
            ResetRFID();
            break;
        }
        }
        else 
        {
             humain = false;
          /*  for(int i =0;i<2;i++)
            {
            humainDetecter = SONAR_GetRange(1) < 40;
            Serial.print(humainDetecter);
            
            if(humainDetecter) humain = true;
             }*/
             humain =true;
            if(humain)SuivreLigne();
            else
            {
             MOTOR_SetSpeed(RIGHT,0);
            MOTOR_SetSpeed(LEFT,0);

            }
            
        }
     
    }
}