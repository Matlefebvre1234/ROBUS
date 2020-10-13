#include <Arduino.h>
#include <LibRobus.h>

// déclaration des variables
#define RAYON 9.5 //rayon entre les deux roues en cm
#define PI 3.1416
#define FRONT 0
#define TURN 1
#define kp 0.0001
#define ki 0.00002
#define kpB 0.0004
#define kiB 0.00004

// définition des variables de vitesse pour Robus
float g_vit_motg_Origin = 0.35;
float g_vit_motd_Origin = 0.35;
float g_vit_Acc = .75;
float g_vit_motd = g_vit_motd_Origin;
float g_vit_motg = g_vit_motg_Origin;

// variables globale
int g_direction = 1;
int g_pulse_derniere_valeurLu_g = 0;
int g_dist_reel_totD = 0;
int g_dist_totalG = 0 ;
int gt_dist_total_reelG_D[2] = {0,0};
int gt_derniere_lu_G_D[2] = {0,0};
int g_dist_reel_totG = 0 ;
int g_step = 0; 

// fonction existante
bool Tourner(float degre);
void Virage_1roue(float angle);
void Virage_2roue(float angle);
void Avancer(float distance);
void LigneDroitePID2();
void reinitialiserVariable();

void setup()
{
    BoardInit();
    Serial.begin(9600);
    reinitialiserVariable();
}
// fonction pour reset les variables que nous utilisons pour le PID
void reinitialiserVariable()
{
    g_vit_motd = g_vit_motd_Origin;
    g_vit_motg = g_vit_motg_Origin;
    g_dist_reel_totD = 0;
    g_dist_totalG = 0;
    g_dist_reel_totG = 0;
    g_pulse_derniere_valeurLu_g = 0;
    gt_dist_total_reelG_D[LEFT] = 0;
    gt_dist_total_reelG_D[RIGHT] = 0;
    gt_derniere_lu_G_D[LEFT] = 0;
    gt_derniere_lu_G_D[RIGHT] = 0;
    g_direction = 1;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
}
// fait avance en ligne droite le robot selon les constantes globales
// set la vitesse selon le PI.
void LigneDroitePID2()
{
    // roue gauche maitre roue droite esclave ou vise versa
    float erreurDistanceD;
    float erreurDistanceG;
    float comp_d;
    float comp_g;
    float erreurD;
    float erreurG;
    int vitesseLuD = ENCODER_ReadReset(RIGHT);
    int vitesseLuG = ENCODER_ReadReset(LEFT);
    
    // ajout au total calculé en référence à la dernière réinitiliasation des variables
    g_pulse_derniere_valeurLu_g = vitesseLuG;
    gt_derniere_lu_G_D[LEFT] = vitesseLuG;
    gt_derniere_lu_G_D[RIGHT] = vitesseLuD;
    g_dist_totalG += vitesseLuG;
    gt_dist_total_reelG_D[RIGHT] += vitesseLuD;
    gt_dist_total_reelG_D[LEFT] += vitesseLuG;
    g_dist_reel_totD += vitesseLuD;
    g_dist_reel_totG += vitesseLuG;
    // NOUVEAU
    // trouve la direction vers la roue tourne pour chacune ( avant positif, arière négatif)
    int direction_droite = g_vit_motd > 0 ? 1 : -1;        
    int direction_gauche = g_vit_motg > 0 ? 1 : -1;

    // calcule la différence pour la vitessse pour la roue de droite et gauche
    erreurG = abs(vitesseLuG) - abs(vitesseLuD);
    erreurD = abs(vitesseLuD) - abs(vitesseLuG);
    erreurDistanceD =  abs(g_dist_reel_totD) - abs(g_dist_reel_totG);
    erreurDistanceG =  abs(g_dist_reel_totG) - abs(g_dist_reel_totD);

    // dépendant des robots car elles sont composées de matériaux différents,
    // calcule de la compatation pour chacune des roues.
    comp_d = kp * erreurD + ki * erreurDistanceD; // Robot A
     comp_g = kp * erreurG + ki * erreurDistanceG;
  //  comp_d = kpB * erreurD + kiB * erreurDistanceD;   //Robot B
  //  comp_g = kpB * erreurG + kiB * erreurDistanceG;
    g_vit_motd = g_vit_motd - direction_droite*(comp_d/2);
    g_vit_motg = g_vit_motg - direction_gauche*(comp_g/2);
    // FIN NOUVEAU
    Serial.println("compatation is: " + String(comp_d) + " vitesse droite: " + String(g_vit_motd));
    Serial.println("compatation is: " + String(comp_g) + " vitesse gauche: " + String(g_vit_motg));
    MOTOR_SetSpeed(RIGHT,g_vit_motd);
    MOTOR_SetSpeed(LEFT,g_vit_motg);
}
// fait avancer Robus pour le nombre de pulse passé en variable.
void Avancer(int pulse)
{
    float deceleration = pulse - (pulse * .2); 
    float acceleration = pulse * .1;
    float speed_acc_d;
    float speed_acc_g;
    bool changed_dec = false;
    bool changed_acc = false;
    while(g_dist_reel_totG < pulse - g_pulse_derniere_valeurLu_g)
    {
        // NOUVEAU
        if(!changed_acc && g_dist_reel_totG >= acceleration) {
            changed_acc = true;
            speed_acc_d = g_vit_motd*g_vit_Acc;
            speed_acc_g = g_vit_motg*g_vit_Acc;
            g_vit_motd = g_vit_motd + speed_acc_d;
            g_vit_motg = g_vit_motg + speed_acc_g;
            Serial.println(g_vit_motg);
        } else if(!changed_dec && g_dist_reel_totG >= deceleration) {
            changed_dec = true;
            g_vit_motd = g_vit_motd - speed_acc_d;
            g_vit_motg = g_vit_motg - speed_acc_g;
        }
        // FIN NOUVEAU
        LigneDroitePID2();
    }
    MOTOR_SetSpeed(RIGHT,0);
    MOTOR_SetSpeed(LEFT,0);
}
// fonction TOURNER, les COOL
bool Tourner(float degre)
{
    float arc = (degre*PI*RAYON)/180;
    float pulse = (arc*3200.0)/23.88;
    pulse = fabs(pulse);
    static bool resetVar = 0;
    if(resetVar == 0)
    {
        resetVar ++;
        ENCODER_Reset(RIGHT);
        ENCODER_Reset(LEFT);
    }
    if (degre < 0)
    {
        MOTOR_SetSpeed(RIGHT,0.15);
        MOTOR_SetSpeed(LEFT,-0.15);
    }
    else
    {
        MOTOR_SetSpeed(RIGHT,-0.15);
        MOTOR_SetSpeed(LEFT,0.15);
    }
    if(abs(ENCODER_Read(LEFT)) < pulse)
    {
        return 1; // return 1 if not finish
    }
    else
    {
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
        resetVar = 0;
        return 0;
    }
}
// virage avec une roue la sur laquelle elle tourne est la gauche.
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
// virage en utilisant les deux, selon le signe de l'angle Robus tournera à gauche en recevant un angle de -90;
// Robus tournera à droite en recevant un angle de 90
void Virage_2roue(float angle)
{
    // la roue maître est celle qui fait une rotation au sens horaire d'une montre (va vers l'avant)
    int g_direction = 1;
    int roue_maitre = LEFT;
    if(angle < 0)
    {
        g_direction = -1;
        roue_maitre = RIGHT;
    }

    // arrête Robus et remets à zéro les encodeurs.
    MOTOR_SetSpeed (RIGHT, 0);
    MOTOR_SetSpeed (LEFT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    // calcul du nombre de pulse à faire selon un chiffre magique.
    // et la variable global de direction défini la roue allant vers l'avant et la roue allant vers l'arrière.
    float arc = (abs(angle)*PI*RAYON)/180;
    int pulse_distribution = (arc*3200.0)/23.88;

    g_vit_motd = g_direction * -0.15;
    g_vit_motg = g_direction * 0.15;
    
    Serial.print("Encodeur de la roue allant vers l'avant (" + String(roue_maitre) + "): ");
    Serial.println(abs(ENCODER_Read(roue_maitre)));
    Serial.print("Nombre de pulse pour chaque roue: ");
    Serial.println(pulse_distribution);

    // tant que l'encodeur de la roue maître est plus petite que le pulse cible moins la dernière valeur lue pour
    // la roue maître.
    while(abs(gt_dist_total_reelG_D[roue_maitre]) < pulse_distribution - abs(gt_derniere_lu_G_D[roue_maitre]))
    {
        // appel de la fonction qui avance Robus et rafraichit les variables avec lesquelles Robus
        // peut ajuster ses deux roues pour tourner avec la précision que la fonction lui permet.
        LigneDroitePID2();
    }
    MOTOR_SetSpeed (LEFT, 0);
    MOTOR_SetSpeed (RIGHT, 0);
    delay(500);
    Serial.println(pulse_distribution);
    Serial.println(gt_dist_total_reelG_D[roue_maitre]);
    Serial.println(gt_derniere_lu_G_D[roue_maitre]);

    int pulse_erreur = pulse_distribution - abs(gt_dist_total_reelG_D[roue_maitre]) - abs(gt_derniere_lu_G_D[roue_maitre]);
    Serial.println(pulse_erreur);
    if(pulse_erreur != 0){
        MOTOR_SetSpeed(roue_maitre, -0.35);
        delay(50);
        ENCODER_Reset(roue_maitre);
        while (abs(ENCODER_Read(roue_maitre)) < abs(pulse_erreur))
        {
            Serial.println(abs(ENCODER_Read(roue_maitre)));
        }
        MOTOR_SetSpeed (roue_maitre, 0);
    }
}
// converti les centimètres en pulse à faire pour Robus
int CmEnPulse (int distance_cm) 
{
    int distancePulse = (3200/23.9389*distance_cm);
    return distancePulse;
}
// main loop faisant le circuit
void loop()
{    
    int instructions[11][2] = {
        {128, FRONT},
        {-90, TURN},
        {94, FRONT},
        {90, TURN},
        {85, FRONT},
        {50, TURN},
        {185, FRONT},
        {-90, TURN},
        {60, FRONT},
        {48, TURN},
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
            delay(50);
            break;
        
        case TURN:
            Serial.println("turn");
            instructions[i][0] = -distance_degre;
            Virage_2roue(distance_degre);
            delay(50);
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