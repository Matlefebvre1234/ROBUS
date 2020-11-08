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
    Serial.begin(9600);
    reinitialiserVariable();
}
void loop()
{

}