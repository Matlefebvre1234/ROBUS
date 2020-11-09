#include <Arduino.h>
#include <LibRobus.h>
#include <musique.h>
#include <RobusDarthGenius.h>
void setup()
{
    // BOILER PLATE SETUP
    BoardInit();
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
    Avancer(CmEnPulse(25), false);
    reinitialiserVariable();
    Virage_2roue(-90);
    reinitialiserVariable();
    Avancer(CmEnPulse(70), false);
    delay(1000); //couleur
    reinitialiserVariable();
    Avancer(CmEnPulse(150), false);
    reinitialiserVariable();
    AttraperBalle();
    delay(1000);

    if (0) //jaune
    {
        Avancer(CmEnPulse(84), false);
        reinitialiserVariable();
        Virage_2roue(93);
        reinitialiserVariable();
        Avancer(CmEnPulse(25), false);
        reinitialiserVariable();
    }
    else if (1) //bleu
    {
        Avancer(CmEnPulse(163), false);
        reinitialiserVariable();
        Virage_2roue(-90);
        reinitialiserVariable();
        Avancer(CmEnPulse(25), false);
        reinitialiserVariable();
    }
    else if (0) //rouge
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
// Serial.println("dans fonction loop");
// reinitialiserVariable();
// distanceSonar= SONAR_GetRange(1);
// delay(500);
// Serial.println(distanceSonar);
float distanceSonar = 0;
distanceSonar = SONAR_GetRange(1);
Avancer(CmEnPulse(200), true);
Avancer(CmEnPulse(200), true);
Avancer(CmEnPulse(75), true);
delay(500);
exit(0);

// while ((distanceSonar >= 90)||(distanceSonar == 0))
// {   
//     distanceSonar= SONAR_GetRange(1);
//     Avancer(CmEnPulse(i));
//     j = i + j;
//     Serial.print(j);
//     Serial.print("     ");
//     Serial.println(distanceSonar);
//     if (j >= 480)
//     {
//         Serial.println("Fuck");
//         Virage_2roue(-90);
//         Avancer(CmEnPulse(80));
//     }
// }

// Virage_2roue(-90);

// Virage_2roue(-90);
//     Avancer(CmEnPulse(80));
// Virage_2roue(distance_degre);
// delay(10);
    
    // while(1)
//sensualSong();
delay(500);
}