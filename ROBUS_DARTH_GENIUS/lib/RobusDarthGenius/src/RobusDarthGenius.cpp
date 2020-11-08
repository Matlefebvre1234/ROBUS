#include <Arduino.h>
#include <LibRobus.h>
#include <RobusDarthGenius.h>

int g_direction = 1;
int vitesse = 300;
float g_vit_motg_Origin = 1;
float g_vit_motd_Origin = 1;
float g_vit_motd = g_vit_motd_Origin;
float g_vit_motg = g_vit_motg_Origin;
int nbcycle = 0;
float dist_reel_totD = 0;
float dist_totalG =0 ;
int dist_reel_totG = 0;
float derniereValeurLuGPulse = 0;
int gt_dist_total_reelG_D[2] = {0,0};
int gt_derniere_lu_G_D[2] = {0,0};
float kp = 0.0002;
float ki = 0.00004;
float kpB_Origine = 0.0004;
float kiB_Origine = 0.00004;
float kpB = kpB_Origine;
float kiB = kiB_Origine;

// fonction pour reset les variables que nous utilisons pour le PID
void reinitialiserVariable()
{
    g_vit_motd = g_vit_motd_Origin;
    g_vit_motg = g_vit_motg_Origin;
    kpB = kpB_Origine;
    kiB = kiB_Origine;
    nbcycle = 0;
    dist_reel_totD = 0;
    dist_reel_totG = 0 ;
    dist_totalG = 0;
    derniereValeurLuGPulse = 0;
    gt_dist_total_reelG_D[LEFT] = {0};
    gt_dist_total_reelG_D[RIGHT] = {0};
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
    float vitesseLuD = ENCODER_ReadReset(RIGHT);
    float vitesseLuG = ENCODER_ReadReset(LEFT);
    
    // ajout au total calculé en référence à la dernière réinitiliasation des variables
    derniereValeurLuGPulse = vitesseLuG;
    gt_derniere_lu_G_D[RIGHT] = vitesseLuD;
    gt_derniere_lu_G_D[LEFT] = vitesseLuG;
    gt_dist_total_reelG_D[RIGHT] += vitesseLuD; 
    gt_dist_total_reelG_D[LEFT] += vitesseLuG;
    dist_totalG += vitesseLuG;
    dist_reel_totD += vitesseLuD;
    dist_reel_totG += vitesseLuG;

    // trouve la direction vers la roue tourne pour chacune ( avant positif, arière négatif)
    int direction_droite = g_vit_motd > 0 ? 1 : -1;
    int direction_gauche = g_vit_motg > 0 ? 1 : -1;

    // calcule la différence pour la vitessse pour la roue de droite et gauche
    erreurG = abs(vitesseLuG) - abs(vitesseLuD);
    erreurD = abs(vitesseLuD) - abs(vitesseLuG);
    erreurDistanceD =  abs(dist_reel_totD) - abs(dist_reel_totG);
    erreurDistanceG =  abs(dist_reel_totG) - abs(dist_reel_totD);

    // dépendant des robots car elles sont composées de matériaux différents,
    // calcule de la compatation pour chacune des roues.
    
    //comp_d = kp * erreurD + ki * erreurDistanceD; // Robot A
    //comp_g = kp * erreurG + ki * erreurDistanceG;
    comp_d = kpB * erreurD + kiB * erreurDistanceD;   //Robot B
    comp_g = kpB * erreurG + kiB * erreurDistanceG;
    
    // FIN de la dependance pour PID
    g_vit_motd = g_vit_motd - direction_droite*(comp_d/2);
    g_vit_motg = g_vit_motg - direction_gauche*(comp_g/2);


    // FIN NOUVEAU
    delay(30);

    MOTOR_SetSpeed(RIGHT,g_vit_motd);
    MOTOR_SetSpeed(LEFT,g_vit_motg);
}

void Avancer(int pulse)
{
    int deceleration = pulse - (pulse * 0.1);
    float acceleration_v;
    float v_max = g_vit_motg_Origin;
    float n_pulse_bar = 4200;
    bool accel = true;


    g_vit_motd = 0.22;
    g_vit_motg = 0.22;

    if(pulse <= CmEnPulse(60)) {
        v_max = .30;
    }

    acceleration_v = (v_max - g_vit_motd)/n_pulse_bar;
    Serial.println("DÉBUT: " + String(pulse));
    LigneDroitePID2();
    int ki_correction_in_progress = 0;
    while(dist_reel_totG < pulse - derniereValeurLuGPulse)
    {        
        // acceleration du début
        if (!accel) {
            // rien faire car l'acceleration est terminée
        } else if(g_vit_motg >= v_max && accel == true) {
            accel = false;
        } else if (ki_correction_in_progress < 3) {
            ki_correction_in_progress++;
            // perte de un tour d'acceleration pour laisser le PID se calibrer
        } else {
            // augmentation de la vitesse
            g_vit_motg += acceleration_v*gt_derniere_lu_G_D[LEFT];
            g_vit_motd += acceleration_v*gt_derniere_lu_G_D[RIGHT];
            ki_correction_in_progress = 0;
        }

        //decceleration
        if(accel){

        }
        else if (ki_correction_in_progress < 3) {
            ki_correction_in_progress++;
            // perte de un tour d'acceleration pour laisser le PID se calibrer
        }
        else if(pulse - dist_reel_totG < n_pulse_bar && g_vit_motd > 0 && g_vit_motd > 0 )
        {
            g_vit_motg -= acceleration_v*gt_derniere_lu_G_D[LEFT]*ki_correction_in_progress;
            g_vit_motd -= acceleration_v*gt_derniere_lu_G_D[RIGHT]*ki_correction_in_progress;
            ki_correction_in_progress = 0;
        }
        LigneDroitePID2();
    }
    Serial.println("FIN: " + String(dist_reel_totG));
    MOTOR_SetSpeed(RIGHT,0);
    MOTOR_SetSpeed(LEFT,0);
}

void Virage_1roue(float angle)
{
    int pulse_afaire = angle*44;
    g_direction = 1;
    if(angle < 0)
    {
        g_direction = -1;
    }

    MOTOR_SetSpeed (RIGHT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed (LEFT, g_direction*0.2);
    while(ENCODER_Read(LEFT) < pulse_afaire) { }
    MOTOR_SetSpeed (LEFT, 0);
    Serial.println(ENCODER_Read(LEFT));   
    delay(10);    
}

void Virage_2roue(float angle)
{    
    int g_direction = 1;
    int roue_maitre = LEFT;
    if(angle < 0)
    {
        g_direction = -1;
        roue_maitre = RIGHT;
    }
    MOTOR_SetSpeed (RIGHT, 0);
    MOTOR_SetSpeed (LEFT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    int pulse_afaire = abs(angle)*42.6;
    int pulse_distribution = pulse_afaire/2;

    g_vit_motd = g_direction * -0.14;
    g_vit_motg = g_direction * 0.14;
    
    Serial.print("abs_encoder: ");
    Serial.println(abs(ENCODER_Read(roue_maitre)));
    Serial.print("pulse_afaire/2: ");
    Serial.println(pulse_distribution);

    LigneDroitePID2();
    while(abs(ENCODER_Read(roue_maitre)) <= pulse_distribution - abs(gt_dist_total_reelG_D[roue_maitre]))
    {
        LigneDroitePID2();
    }
    MOTOR_SetSpeed (LEFT, 0);
    MOTOR_SetSpeed (RIGHT, 0);
    delay(500);
    Serial.println(pulse_distribution);
    Serial.println(gt_dist_total_reelG_D[roue_maitre]);
    Serial.println(gt_derniere_lu_G_D[roue_maitre]);

    // CORRECTION D'ERREUR
    int pulse_erreur_g = pulse_distribution - abs(gt_dist_total_reelG_D[LEFT]) - abs(gt_derniere_lu_G_D[LEFT]);
    int pulse_erreur_d= pulse_distribution - abs(gt_dist_total_reelG_D[RIGHT]) - abs(gt_derniere_lu_G_D[RIGHT]);
    if(pulse_erreur_d != 0){
        MOTOR_SetSpeed(RIGHT, (roue_maitre == RIGHT ? -.30: .30));
        pulse_erreur_d = abs(pulse_erreur_d) * 2;
        ENCODER_Reset(RIGHT);
        while (abs(ENCODER_Read(RIGHT)) < pulse_erreur_d)
        {
            Serial.println(abs(ENCODER_Read(RIGHT)));
        }
        MOTOR_SetSpeed (RIGHT, 0);
    }
    // FIN DE correction derreur
}
int CmEnPulse (int distance_cm) 
{
    int distancePulse = (3200/23.9389*distance_cm);
    return distancePulse;
}