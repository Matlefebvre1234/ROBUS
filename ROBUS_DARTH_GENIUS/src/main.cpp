#include <Arduino.h>
#include <LibRobus.h>

#define PI 3.1416
int delais = 100;
int vitesse = 300;
float vit_motg_Origin = 0.35;
float vit_motd_Origin = 0.35;
float vit_motd = vit_motd_Origin;
float vit_motg = vit_motg_Origin;
float derniereValeurLuGPulse = 0;
int nbcycle = 0;
float dist_reel_totD = 0;
float dist_totalG =0 ;
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


void reinitialiserVariable()
{
 vit_motd = vit_motd_Origin;
 vit_motg = vit_motg_Origin;
 nbcycle = 0;
 dist_reel_totD = 0;
 dist_totalG =0 ;
 dist_reel_totG =0 ;
derniereValeurLuGPulse = 0;
}
void setup()
{
    
    BoardInit();
    Serial.begin(9600);
    reinitialiserVariable();
}


void LigneDroitePID2()
{
    //roue gauche maitre roue droite esclave
    float erreurDistanceD;
    float erreurDistanceG;
    float comp;
    float compG;
    float vitesseLuD = ENCODER_ReadReset(RIGHT);
    float vitesseLuG = ENCODER_ReadReset(LEFT);
    
    derniereValeurLuGPulse = vitesseLuG;
   
    delay(delais);
   
    float erreurD = vitesseLuD - vitesseLuG;
    dist_totalG += vitesseLuG;
    dist_reel_totD += vitesseLuD;
    dist_reel_totG += vitesseLuG;
    
    erreurDistanceD =  dist_reel_totD- dist_reel_totG;
   
    //comp = kp * erreurD + ki * erreurDistanceD; // Robot A
    comp = kpB * erreurD + kiB * erreurDistanceD;   //Robot B

    vit_motd = vit_motd - comp;
  
   /*Serial.print("vitesseLuG = ");
    Serial.print(vitesseLuG);
    Serial.print("  vitesseLuD = ");
    Serial.print(vitesseLuD);
    Serial.print("  erreurD = ");
    Serial.print(erreurD);
    Serial.print("  distTotalG = ");
    Serial.print(dist_reel_totG);
    Serial.print("  distReelD = ");
    Serial.print(dist_reel_totD);
    Serial.print("  comp = ");
    Serial.print(comp);
    Serial.print("  vitesseMotG = ");
    Serial.print(vit_motg);
    Serial.print("  vitesseMotD = ");
    Serial.print(vit_motd);
    Serial.print("\n");*/

    MOTOR_SetSpeed(RIGHT,vit_motd);
    MOTOR_SetSpeed(LEFT,vit_motg);

}

void Avancer(int pulse)
{
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
    int direction = 1;
    if(angle < 0)
    {
        direction = -1;
    } 
    MOTOR_SetSpeed (RIGHT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    int pulse_afaire = angle*44;

    MOTOR_SetSpeed (LEFT, direction*0.2);
    while(ENCODER_Read(LEFT) < pulse_afaire)
    {
      
    }
    Serial.println(ENCODER_Read(LEFT));
    MOTOR_SetSpeed (LEFT, 0);
    delay(10);
    
}

void Virage_2roue(float angle)
{

    int direction = 1;
    if(angle < 0)
    {
        direction = -1;
    } 

    MOTOR_SetSpeed (RIGHT, 0);
    MOTOR_SetSpeed (LEFT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    int pulse_afaire = abs(angle)*44;

    MOTOR_SetSpeed (LEFT, direction * 0.15);
    MOTOR_SetSpeed(RIGHT, direction * -0.15);
    
    Serial.print("abs_encoder:");
    Serial.println(abs(ENCODER_Read(LEFT)));
    Serial.print("pulse_afaire/2");
    Serial.println(pulse_afaire/2);

    while(abs(ENCODER_Read(LEFT)) < pulse_afaire/2)
    {
        Serial.println("here");
    }
    Serial.println(ENCODER_Read(LEFT));
    MOTOR_SetSpeed (LEFT, 0);
    MOTOR_SetSpeed (RIGHT, 0);
    delay(10);
}
int CmEnPulse (int distance_cm) 
{
    int distancePulse = (3200/24*distance_cm);
    return distancePulse;
}
void loop()
{
    Serial.println("MONCODEulse:");
    int nmPulse = CmEnPulse(145);
    Serial.println("nm pulse:" + String(nmPulse));
    Avancer(nmPulse);
  //   reinitialiserVariable();
// Virage_2roue(180);
  //   Avancer(100);
     exit(0);
}