#include <Arduino.h>
#include <LibRobus.h>
<<<<<<< Updated upstream
#include <musique.h>
#include <RobusDarthGenius.h>
=======
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
// déclaration des variables
#define PI 3.1416
#define FRONT 0
#define TURN 1
int g_direction = 1;
int vitesse = 300;

// VITESSE pour le robot B a .50
// VITESSE pour le robot A a .66 - .75
float g_vit_motg_Origin = 0.66;
float g_vit_motd_Origin = 0.66;

float g_vit_motd = g_vit_motd_Origin;
float g_vit_motg = g_vit_motg_Origin;
float derniereValeurLuGPulse = 0;
int nbcycle = 0;
float dist_reel_totD = 0;
float dist_totalG =0 ;
int gt_dist_total_reelG_D[2] = {0,0};
int gt_derniere_lu_G_D[2] = {0,0};
int dist_reel_totG = 0;
int g_pince_ouverte[2] = {120,125};
int g_pince_fermer[2] = {107,112}; 

// déclaration des PID
float kp = 0.0002;
float ki = 0.00004;
float kpB_Origine = 0.0004;
float kiB_Origine = 0.00004;
float kpB = kpB_Origine;
float kiB = kiB_Origine;

void Virage_1roue(float angle);
void Virage_2roue(float angle);
void Avancer(float distance);
void LigneDroitePID2();
void reinitialiserVariable();
int CmEnPulse (int distance_cm);

bool avertisseurSonore();


//fonctions à mettre avec les autres dans un header
void RelacherBalle()
{
    SERVO_Enable (LEFT);
    SERVO_Enable (RIGHT);

    SERVO_SetAngle (LEFT, g_pince_ouverte[0]);
    SERVO_SetAngle (RIGHT, g_pince_ouverte[1]);
    delay(500);

    SERVO_Disable (LEFT);
    SERVO_Disable (RIGHT);
}
void AttraperBalle()
{

    SERVO_Enable (LEFT);
    SERVO_Enable (RIGHT);

    SERVO_SetAngle (LEFT, g_pince_fermer[0]);
    SERVO_SetAngle (RIGHT, g_pince_fermer[1]);
    delay(500);
}

bool avertisseurSonore()
{    
    int detectedSound = analogRead(A0);
    int bruitAmbiant = analogRead(A1);
    Serial.println("bruit detected: " + String(detectedSound));
    Serial.println("bruit ambiant: " + String(bruitAmbiant));
    return detectedSound > bruitAmbiant;
}
// fonction pour reset les variables que nous utilisons pour le PID
void reinitialiserVariable()
{
    g_vit_motd = g_vit_motd_Origin;
    g_vit_motg = g_vit_motg_Origin;
    kpB = kpB_Origine;
    kiB = kiB_Origine;
    nbcycle = 0;
    dist_reel_totD = 0;
    dist_totalG = 0;
    dist_reel_totG = 0 ;
    derniereValeurLuGPulse = 0;
    gt_dist_total_reelG_D[LEFT] = {0};
    gt_dist_total_reelG_D[RIGHT] = {0};
    g_direction = 1;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
}
>>>>>>> Stashed changes
void setup()
{
    // BOILER PLATE SETUP
    BoardInit();
    Serial.begin(9600);
    reinitialiserVariable();
}
void prendreBallon()
{
<<<<<<< Updated upstream
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
=======
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
    
   // comp_d = kp * erreurD + ki * erreurDistanceD; // Robot A
    //comp_g = kp * erreurG + ki * erreurDistanceG;
    comp_d = kpB * erreurD + kiB * erreurDistanceD;   //Robot B
    comp_g = kpB * erreurG + kiB * erreurDistanceG;
    
    // FIN de la dependance pour PID
    g_vit_motd = g_vit_motd - direction_droite*(comp_d/2);
    g_vit_motg = g_vit_motg - direction_gauche*(comp_g/2);


    // FIN NOUVEAU
    delay(50);

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

    acceleration_v = (v_max - g_vit_motd)/n_pulse_bar *0.3;
    Serial.println("DÉBUT: " + String(pulse));
    LigneDroitePID2();
    while(dist_reel_totG < pulse - derniereValeurLuGPulse)
    {
        // acceleration du début
        if (g_vit_motg < v_max && accel == true)
            g_vit_motg += acceleration_v*gt_derniere_lu_G_D[LEFT];
        else
        {
            accel = false;
        }
        if (g_vit_motd < v_max && accel == true)
            g_vit_motd += acceleration_v*gt_derniere_lu_G_D[RIGHT];
        else
        {
            accel = false;
        }
        
        //decceleration
        if(pulse - dist_reel_totG < n_pulse_bar && g_vit_motd > 0.1 && g_vit_motd > 0.1 )
        {
            g_vit_motg -= acceleration_v*gt_derniere_lu_G_D[LEFT]*1.20;
            g_vit_motd -= acceleration_v*gt_derniere_lu_G_D[RIGHT]*1.20;
            Serial.println("distance reel total G: " + String(dist_reel_totG));
            LigneDroitePID2();
        }
        else
        {
            Serial.println("distance reel total G: " + String(dist_reel_totG));
            LigneDroitePID2();
        }
        
    }
    Serial.println("FIN: " + String(dist_reel_totG));
    MOTOR_SetSpeed(RIGHT,0);
    MOTOR_SetSpeed(LEFT,0);
}
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
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
=======
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
        {125, FRONT},
        {-90, TURN},
        {105, FRONT},
        {90, TURN},
        {46, FRONT},
        {90, TURN},
        {58, FRONT},
        {-90, TURN},
        {205, FRONT},
        {90, TURN},
        {50, FRONT},
        {-89, TURN},
        {130, FRONT}

    };
    SERVO_Disable(LEFT);
        SERVO_Disable(RIGHT);
    delay(1000);
    if(avertisseurSonore()) {
        //reinitialiserVariable();
        g_vit_motd_Origin =0.4;
        g_vit_motg_Origin =0.4;
        RelacherBalle();
        Virage_2roue(95);
        delay(2000);
        reinitialiserVariable();
        Avancer(CmEnPulse(25));
        reinitialiserVariable();
        Virage_2roue(-92);
        reinitialiserVariable();
        Avancer(CmEnPulse(70));
        delay(1000); //couleur
        reinitialiserVariable();
        Avancer(CmEnPulse(150));
        reinitialiserVariable();
        AttraperBalle();

        g_vit_motd_Origin =0.3;
        g_vit_motg_Origin =0.3;
        delay(1000);
       
        if(1) //jaune
        {
             Avancer(CmEnPulse(84));
          reinitialiserVariable();
        Virage_2roue(90);
          reinitialiserVariable();
        Avancer(CmEnPulse(25));
          reinitialiserVariable();



        }
        else if(0)//bleu
        {
        Avancer(CmEnPulse(163));
          reinitialiserVariable();
        Virage_2roue(-90);
          reinitialiserVariable();
        Avancer(CmEnPulse(25));
          reinitialiserVariable();


        }
              else if(0) //rouge
        {
        Avancer(CmEnPulse(230));
          reinitialiserVariable();
        Virage_2roue(90);
          reinitialiserVariable();
        Avancer(CmEnPulse(25));
          reinitialiserVariable();


        }
      
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
        // // FIN de la loop for du retour
    }

>>>>>>> Stashed changes
}