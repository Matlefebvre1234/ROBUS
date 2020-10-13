#include <Arduino.h>
#include <LibRobus.h>

// déclaration des variables
#define RAYON 9.5 //rayon entre les deux roues en cm
int delais = 100;
float vit_motd = 0.35;
float vit_motg = 0.35;
float derniereValeurLuG;
float dist_reel_totD = 0;
float dist_reel_totG = 0;
int step = 0;

// constante pour le pid
float kp = 0.0001;
float ki = 0.00002;
float kpB = 0.0004;
float kiB = 0.00004;
 
// déclaration des fonctions
bool Avancer(float distance);
void LigneDroitePID2();
void reinitialiserVariable();
bool Tourner(float degre);

void setup()
{
    BoardInit();
    Serial.begin(9600);    
    delay(100);
    reinitialiserVariable();
    delay(200);
}

// fonction pour reset les variables que nous utilisons pour le PID
void reinitialiserVariable()
{
 vit_motd = 0.35;
 vit_motg = 0.35;
 dist_reel_totD = 0;
 dist_reel_totG = 0;
 dist_reel_totD = 0;
 ENCODER_Reset(RIGHT);
 ENCODER_Reset(LEFT);
}

// fait avance en ligne droite le robot selon les constantes globales
// set la vitesse selon le PI.
void LigneDroitePID2()
{
    // roue gauche maitre roue droite esclave
    float erreurDistanceD = 0;
    float comp = 0;
    float vitesseLuD = ENCODER_ReadReset(RIGHT);
    float vitesseLuG = ENCODER_ReadReset(LEFT);

    Serial.print(vitesseLuD);
    Serial.print("   ");
    Serial.println(vitesseLuG);
    
    derniereValeurLuG = vitesseLuG;   
    delay(delais);   
    float erreurD = vitesseLuD - vitesseLuG;
    //dist_totalG += vitesseLuG;
    dist_reel_totD += vitesseLuD;
    dist_reel_totG += vitesseLuG;
    
    erreurDistanceD =  dist_reel_totD - dist_reel_totG;
   
    // dépendant des robots car ils sont composés de différents matériaux
    //comp = kp * erreurD + ki * erreurDistanceD; // Robot A
    comp = kpB * erreurD + kiB * erreurDistanceD; //Robot B

/* 
    Serial.print(erreurD);
    Serial.print("   ");
    Serial.println(erreurDistanceD);
    */

    vit_motd = vit_motd - comp;
    MOTOR_SetSpeed(RIGHT,vit_motd);
    MOTOR_SetSpeed(LEFT,vit_motg);

}

// fait avancer le robot pour une (distance) en appellant la fonction LigneDroitePID2
bool Avancer(float distance)
{
    float distanceParcouru = dist_reel_totG;
    distanceParcouru = 2 * PI * 3.81 * distanceParcouru / (3200);   
    derniereValeurLuG = 2 * PI *3.81 * derniereValeurLuG / (3200);

    // Tant que la distance parcourue est plus petite que la distance a parcourir moins 
    // la derniere valeur lue, appel la fonction ligne droite (pid).

    if(distanceParcouru < distance - derniereValeurLuG)
    {
        LigneDroitePID2();
        return 1;
    }
    else
    {
        // arrête le robot apres avoir parcouru la distance voulue
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
        return 0;
    }
}

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
        reinitialiserVariable();
        return 0; // return 0 if you suck.
    }
}


void loop()
{
    switch (step)
    {
    case 0:
        if(Avancer(125) == 0)
        {
            step ++;
        }
        break;
    case 1:
        if(Tourner(-90) == 0)
        {
            step ++;
        }
        break;
    case 2:
        if(Avancer(90) == 0)
        {
            step ++;
        }
        break;
    case 3:
        if(Tourner(90) == 0)
        {
            step ++;
        }
    case 4:
        if(Avancer(90) == 0)
        {
            step ++;
        }
        break;
    case 5:
        if(Tourner(45) == 0)
        {
            step ++;
        }
        break;   
    case 6:
        if(Avancer(185) == 0)
        {
            step ++;
        }
        break;
    case 7:
        if(Tourner(-90) == 0)
        {
            step ++;
        }
        break; 
    case 8:
        if(Avancer(64) == 0)
        {
            step ++;
        }
        break;
    case 9:
        if(Tourner(45) == 0)
        {
            step ++;
        }
        break; 
    case 10:
        if(Avancer(100) == 0)
        {
            step ++;
        }
        break;
    default:
        break;
    }
}