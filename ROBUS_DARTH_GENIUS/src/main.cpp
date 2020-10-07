#include <Arduino.h>
#include <LibRobus.h>

#define PI 3.1416
int delais = 100;
int vitesse = 300;
float vit_motg_Origin = 0.25;
float vit_motd_Origin = 0.25;
float vit_motd = vit_motd_Origin;
float vit_motg = vit_motg_Origin;
float derniereValeurLuG;
int nbcycle = 0;
float dist_reel_totD = 0;
float dist_totalG =0 ;
float dist_reel_totG =0 ;
float dist_theorique =0 ;
 float kp = 0.0001;
 float ki = 0.00002;
 float kpG = 0.000001;
 float kiG = 0.00001;



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
 dist_theorique =0 ;






}
void setup()
{
    
    BoardInit();
    Serial.begin(9600);
    
    delay(1000);
    reinitialiserVariable();
    delay(2000);
    Serial.print("TAMERE");
   
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
    
    derniereValeurLuG = vitesseLuG;
    //if(dist_reel_totG >= 1600)
   // {
    delay(delais);
    //float erreurG = vitesseLuG - vitesse;
    float erreurD = vitesseLuD - vitesseLuG;
    dist_totalG += vitesseLuG;
    dist_theorique += vitesse;
    dist_reel_totD += vitesseLuD;
    dist_reel_totG += vitesseLuG;
    
    
    erreurDistanceD =  dist_reel_totD- dist_reel_totG;
   // erreurDistanceG = dist_reel_totG - dist_theorique;
    comp = kp * erreurD + ki * erreurDistanceD;
    //compG = kpG * erreurG;

    vit_motd = vit_motd - comp;
  //  vit_motg = vit_motg - compG;
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
   //}
  /*  else
    {
        vit_motd = vit_motd_Origin;
        vit_motg = vit_motg_Origin;
    MOTOR_SetSpeed(LEFT,vit_motg_Origin/2);
    MOTOR_SetSpeed(RIGHT,vit_motd_Origin/2);
    delay(delais);
    vitesseLuD = ENCODER_ReadReset(RIGHT);
    vitesseLuG = ENCODER_ReadReset(LEFT);
    dist_reel_totG += vitesseLuG;
    dist_reel_totD += vitesseLuD;
    float erreurD = vitesseLuD - vitesseLuG;
    //erreurDistanceD = dist_reel_totD - dist_reel_totG;
   // comp = kp * erreurD;
    //vit_motd = vit_motd - comp; 
   //MOTOR_SetSpeed(RIGHT,vit_motd);

    Serial.print("  vitesseMotG = ");
    Serial.print(vit_motg);
    Serial.print("  vitesseMotD = ");
    Serial.print(vit_motd);
    Serial.print("  vitesseLuG = ");
    Serial.print(vitesseLuG);
    Serial.print("  vitesseLud = ");
    Serial.print(vitesseLuD);
      Serial.print("  comp = ");
    Serial.print(comp);
    Serial.print("\n");
    //delay(500);
    }*/
    
}

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
    if(distanceParcouru < distance -derniereValeurLuG)
    {
        LigneDroitePID2();
    }
    else
    {
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
      /*  dist_reel_totD = 0;
        dist_totalG =0 ;
        dist_reel_totG =0 ;
        vit_motd = vit_motd_Origin;
        vit_motg = vit_motg_Origin;*/
    }
    


}

/*void LigneDroitePID (void)
{
    int dist_tot = vitesse * nbcycle;
    float kp = 0.00001;
    float ki = 0.000002;
    double comp = 0;

    
    int vitesselu_g = ENCODER_ReadReset (LEFT);
    int vitesselu_d = ENCODER_ReadReset (RIGHT);

    int erreur_g = vitesse - vitesselu_g;
    int erreur_d = vitesse - vitesselu_d;

    int erreur_vit = erreur_g - erreur_d;
    int erreur_dist = (dist_tot) - dist_reel_tot;

    comp = ((double)erreur_vit * kp) + ((double)erreur_dist * ki);

    vit_motg = vit_motg + (comp / 2);
    vit_motd = vit_motd - (comp / 2);

    MOTOR_SetSpeed (LEFT, vit_motg);
    MOTOR_SetSpeed (RIGHT, vit_motd);
    dist_reel_tot = dist_reel_tot + (vitesse + ((vitesselu_g - vitesselu_d)/ 2));

    Serial.print("vitesseLuG = ");
    Serial.print(vitesselu_g);
    Serial.print("  vitesseLuD = ");
    Serial.print(vitesselu_d);
    Serial.print("  erreurG = ");
    Serial.print(erreur_G);
    Serial.print("  distTotalG = ");
    Serial.print(dist_totalG);
    Serial.print("  distReelD = ");
    Serial.print(dist_reel_totD);

}*/
void loop()
{
     //delay (delais);
     Avancer(145);
   // LigneDroitePID();
    nbcycle = nbcycle + 1;
    
   
}