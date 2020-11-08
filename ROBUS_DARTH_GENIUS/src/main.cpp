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
void loop()
{
    ///////////////////////////////////////////////////////////////
    // angle positive doit etre mise plus grande pour le robot B
    //
    // ROBOT B pour (+) 10 doit     (+) 10
    // ROBOT B pour (+) 15 doit     (+) 18
    // ROBOT B pour (+) 40 doit     (+) 45
    // ROBOT B pour (+) 45 doit     (+) 47.5
    // ROBOT B pour (+) 90 doit     (+) 95
    // ROBOT B pour (+) 180 doit    (+) 190
    // ROBOT B pour (+) 100 doit    (+) 106
    //
    ///////////////////////////////////////////////////////////////
    //
    // RETOUR angle positive ROBOT B doit etre 190
    //
    int ang_retour = -180;
    ///
    // INSTRUCTIONS
    int instructions[13][2] = {
        {200, FRONT},
        {-180, TURN},
        {200, FRONT},
        {-180, TURN},
        {200, FRONT},
        {-180, TURN},
        {200, FRONT},
        {-180, TURN},
        {200, FRONT},
        {-180, TURN},
        {200, FRONT},
        {-180, TURN},
        {200, FRONT}

    };
    
    delay(1000);
    if(avertisseurSonore()) {
        delay(1000);
        Serial.println("start");
        // for (size_t i = 0; i < 13; i++)
        // {
        //     reinitialiserVariable();
        //     int distance_degre = instructions[i][0];
        //     int action = instructions[i][1];
        //     switch (action)
        //     {
        //     case FRONT:
        //         Serial.println("FRONT");
        //         Avancer(CmEnPulse(distance_degre));
        //         delay(10);
        //         break;
            
        //     case TURN:
        //         Serial.println("TURN");
        //         instructions[i][0] = -distance_degre;
        //         Virage_2roue(distance_degre);
        //         delay(10);
        //         break;
        //     }
        // }

        // // FIN de la loop for
        // reinitialiserVariable();
        // delay(200);
        // Virage_2roue(ang_retour);
        // delay(10);
        // // FIN RETOUR

        // // POUR Y plus grand ou egale au nombre de ligne dans la table
        // for (int Y = 12; Y >= 0; Y--)
        // {
        //     reinitialiserVariable();
        //     int distance_degre = instructions[Y][0];
        //     int action = instructions[Y][1];
        //     switch (action)
        //     {
        //     case FRONT:
        //         Serial.println("FRONT");
        //         Avancer(CmEnPulse(distance_degre));
        //         delay(10);
        //         break;
            
        //     case TURN:
        //         Serial.println("TURN");
        //         Virage_2roue(distance_degre);
        //         delay(10);
        //         break;
        //     }
        // }
        // FIN de la loop for du retour
    }

}