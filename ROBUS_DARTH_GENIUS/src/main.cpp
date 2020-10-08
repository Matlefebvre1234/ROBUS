#include <Arduino.h>
#include <LibRobus.h>

int delais = 1000;
int vitesse = 2000;
float vit_motg = 0.5;
float vit_motd = 0.5;
int nbcycle = 0;
int dist_reel_tot = 0;

void setup()
{
    BoardInit();
}

void LigneDroitePID (void)
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

}
void loop()
{
    LigneDroitePID();
    nbcycle = nbcycle + 1;
    delay (delais);
}