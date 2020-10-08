#include <Arduino.h>
#include <LibRobus.h>
// déclaration des variables
#define PI 3.1416
int delais = 100;
int vitesse = 300;
float vit_motg_Origin = 0.35;
float vit_motd_Origin = 0.35;
float vit_motd = vit_motd_Origin;
float vit_motg = vit_motg_Origin;
float derniereValeurLuG;
int nbcycle = 0;
float dist_reel_totD = 0;
float dist_totalG =0 ;
float dist_reel_totG =0 ;
float kp = 0.0001;
float ki = 0.00002;
float kpB = 0.0004;
float kiB = 0.00004;
// déclaration des fonctions
void Avancer(float distance);
void LigneDroitePID2();
void reinitialiserVariable();
// fonction pour reset les variables que nous utilisons pour le PID
void reinitialiserVariable()
{
 vit_motd = vit_motd_Origin;
 vit_motg = vit_motg_Origin;
 nbcycle = 0;
 dist_reel_totD = 0;
 dist_totalG =0 ;
 dist_reel_totG =0 ;
}
void setup()
{
    // BOILER PLATE SETUP
    BoardInit();
    Serial.begin(9600);    
    delay(1000);
    reinitialiserVariable();
    delay(2000);
}

// fait avance en ligne droite le robot selon les constantes globales
// set la vitesse selon le PI.
void LigneDroitePID2()
{
    // roue gauche maitre roue droite esclave
    float erreurDistanceD;
    float erreurDistanceG;
    float comp;
    float compG;
    float vitesseLuD = ENCODER_ReadReset(RIGHT);
    float vitesseLuG = ENCODER_ReadReset(LEFT);
    
    derniereValeurLuG = vitesseLuG;   
    delay(delais);   
    float erreurD = vitesseLuD - vitesseLuG;
    dist_totalG += vitesseLuG;
    dist_reel_totD += vitesseLuD;
    dist_reel_totG += vitesseLuG;
    
    erreurDistanceD =  dist_reel_totD- dist_reel_totG;
   
    // dépendant des robots car elles sont composées de matériaux différents
    //comp = kp * erreurD + ki * erreurDistanceD; // Robot A
    comp = kpB * erreurD + kiB * erreurDistanceD;   //Robot B

    vit_motd = vit_motd - comp;
    MOTOR_SetSpeed(RIGHT,vit_motd);
    MOTOR_SetSpeed(LEFT,vit_motg);

}
// fait avancer le robot pour une (distance) en appellant la fonction LigneDroitePID2
void Avancer(float distance)
{
    float distanceParcouru = dist_reel_totG;
    distanceParcouru = 2*PI *3.81*distanceParcouru/(3200);
    Serial.print("  distTotalG = ");
    Serial.print(dist_reel_totG);
    Serial.print("  distParcouru = ");
    Serial.print(distanceParcouru);
    Serial.print("\n");    
    derniereValeurLuG = 2*PI *3.81*derniereValeurLuG/(3200);
    // TANT que la distance parcouru est plus petite que la distance a parcourir moins que la derniere valeur lue
    // appel de la fonction ligne droite.
    if(distanceParcouru < distance -derniereValeurLuG)
    {
        LigneDroitePID2();
    }
    else
    {
        // arrête le robot suite que la distance voulue a été
        // parcourue
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
        reinitialiserVariable();
    }
}

void loop()
{
    // avancer le robot pour 1000 cm
    Avancer(1000);
}