#include <Arduino.h>
#include <LibRobus.h>
// déclaration des variables
#define PI 3.1416
#define FRONT 0
#define TURN 1
int g_direction = 1;
int vitesse = 300;
float vit_motg_Origin = 0.45;
float vit_motd_Origin = 0.45;
float vit_motd = vit_motd_Origin;
float vit_motg = vit_motg_Origin;
float derniereValeurLuGPulse = 0;
int nbcycle = 0;
float dist_reel_totD = 0;
float dist_totalG =0 ;
int dist_total_reelG_D[2] = {0,0};
// déclaration des fonctions
int dist_reel_totG =0 ;
float kp = 0.0001;
float ki = 0.00002;
float kpB = 0.0004;
float kiB = 0.00004;

void Virage_1roue(float angle);
void Virage_2roue(float angle);
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
    derniereValeurLuGPulse = 0;
    dist_total_reelG_D[LEFT] = {0};
    dist_total_reelG_D[RIGHT] = {0};
    g_direction = 1;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
}
void setup()
{
    // BOILER PLATE SETUP
    BoardInit();
    Serial.begin(9600);
    reinitialiserVariable();
}

// fait avance en ligne droite le robot selon les constantes globales
// set la vitesse selon le PI.
void LigneDroitePID2()
{
    // roue gauche maitre roue droite esclave ou vise versa
    float erreurDistanceD;
    float erreurDistanceG;
    float comp;
    float comp_d;
    float comp_g;
    float erreurD;
    float erreurG;
    float vitesseLuD = ENCODER_ReadReset(RIGHT);
    float vitesseLuG = ENCODER_ReadReset(LEFT);
    
    // ajout au total calculé en référence à la dernière réinitiliasation des variables
    derniereValeurLuGPulse = vitesseLuG;
    dist_totalG += vitesseLuG;
    dist_total_reelG_D[RIGHT] += vitesseLuD; 
    dist_total_reelG_D[LEFT] += vitesseLuG;
    dist_reel_totD += vitesseLuD;
    dist_reel_totG += vitesseLuG;

    // trouve la direction vers la roue tourne pour chacune ( avant positif, arière négatif)
    int direction_droite = vit_motd > 0 ? 1 : -1;        
    int direction_gauche = vit_motg > 0 ? 1 : -1;

    // calcule la différence pour la vitessse pour la roue de droite et gauche
    erreurG = abs(vitesseLuG) - abs(vitesseLuD);
    erreurD = abs(vitesseLuD) - abs(vitesseLuG);
    erreurDistanceD =  abs(dist_reel_totD) - abs(dist_reel_totG);
    erreurDistanceG =  abs(dist_reel_totG) - abs(dist_reel_totD);

    // dépendant des robots car elles sont composées de matériaux différents,
    // calcule de la compatation pour chacune des roues.
    //comp = kp * erreurD + ki * erreurDistanceD; // Robot A
    comp_d = kpB * erreurD + kiB * erreurDistanceD;   //Robot B
    comp_g = kpB * erreurG + kiB * erreurDistanceG;
    vit_motd = vit_motd - direction_droite*(comp_d/2);
    vit_motg = vit_motg - direction_gauche*(comp_g/2);

    Serial.println("compatation is: " + String(comp_d) + " vitesse droite: " + String(vit_motd));
    Serial.println("compatation is: " + String(comp_g) + " vitesse gauche: " + String(vit_motg));
    MOTOR_SetSpeed(RIGHT,vit_motd);
    MOTOR_SetSpeed(LEFT,vit_motg);
}

void Avancer(int pulse)
{
    Serial.println("DÉBUT: " + String(pulse));
    int deceleration = pulse - (pulse * .1); 
    while(dist_reel_totG < pulse - derniereValeurLuGPulse)
    {
        Serial.println("distance reel total G: " + String(dist_reel_totG));
        LigneDroitePID2();
    }
    Serial.println("FIN: " + String(dist_reel_totG));
    MOTOR_SetSpeed(RIGHT,0);
    MOTOR_SetSpeed(LEFT,0);
}

void Virage_1roue(float angle)
{
    g_direction = 1;
    if(angle < 0)
    {
        g_direction = -1;
    } 
    MOTOR_SetSpeed (RIGHT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    int pulse_afaire = angle*44;

    MOTOR_SetSpeed (LEFT, g_direction*0.2);
    while(ENCODER_Read(LEFT) < pulse_afaire)
    {
      
    }
    Serial.println(ENCODER_Read(LEFT));
    MOTOR_SetSpeed (LEFT, 0);
    delay(10);    
}

void Virage_2roue(float angle)
{    
    int g_direction = 1;
    int roue_maitre = RIGHT;
    if(angle < 0)
    {
        g_direction = -1;
        roue_maitre = LEFT;
    }
    MOTOR_SetSpeed (RIGHT, 0);
    MOTOR_SetSpeed (LEFT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    int pulse_afaire = abs(angle)*41.2526;
    int pulse_distribution = pulse_afaire/2;

    vit_motd = g_direction * -0.15;
    vit_motg = g_direction * 0.15;
    
    Serial.print("abs_encoder: ");
    Serial.println(abs(ENCODER_Read(roue_maitre)));
    Serial.print("pulse_afaire/2: ");
    Serial.println(pulse_distribution);

    while(abs(ENCODER_Read(roue_maitre)) < pulse_distribution - abs(dist_total_reelG_D[roue_maitre]))
    {
        LigneDroitePID2();
    }
    Serial.println(ENCODER_Read(roue_maitre));
    MOTOR_SetSpeed (LEFT, 0);
    MOTOR_SetSpeed (RIGHT, 0);
    delay(10);
}
int CmEnPulse (int distance_cm) 
{
    int distancePulse = (3200/23.9389*distance_cm);
    return distancePulse;
}
void loop()
{    
    int instructions[11][2] = {
        {125, FRONT},
        {-90, TURN},
        {90, FRONT},
        {90, TURN},
        {90, FRONT},
        {45, TURN},
        {200, FRONT},
        {-90, TURN},
        {64, FRONT},
        {45, TURN},
        {100, FRONT}
    };
    for (size_t i = 0; i < 11; i++)
    {
        reinitialiserVariable();
        int distance_degre = instructions[i][0];
        int action = instructions[i][1];
        switch (action)
        {
        case FRONT:
            Serial.println("front");
            Avancer(CmEnPulse(distance_degre));
            delay(10);
            break;
        
        case TURN:
            Serial.println("turn");
            instructions[i][0] = -distance_degre;
            Virage_2roue(distance_degre);
            delay(10);
            break;
        }
    }
    reinitialiserVariable();
    delay(100);
    Virage_2roue(-180);
    delay(10);
    for (int Y = 10; Y > -1; Y--)
    {
        reinitialiserVariable();
        int distance_degre = instructions[Y][0];
        int action = instructions[Y][1];
        switch (action)
        {
        case FRONT:
            Serial.println("front");
            Avancer(CmEnPulse(distance_degre));
            delay(10);
            break;
        
        case TURN:
            Serial.println("turn");
            Virage_2roue(distance_degre);
            delay(10);
            break;
        }
    }
    exit(0);
}