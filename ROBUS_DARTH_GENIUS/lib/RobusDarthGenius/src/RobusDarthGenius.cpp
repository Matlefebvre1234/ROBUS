#include <Arduino.h>
#include <LibRobus.h>
#include <RobusDarthGenius.h>
#include <Adafruit_TCS34725.h>
#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>


bool steady = true;
int g_direction = 1;
int vitesse = 300;
float g_vit_motg_Origin = .10;
float g_vit_motd_Origin = .10;
float g_vit_motd = g_vit_motd_Origin;
float g_vit_motg = g_vit_motg_Origin;
int nbcycle = 0;
long dist_reel_totD = 0;
long dist_totalG = 0;
long dist_reel_totG = 0;
long derniereValeurLuGPulse = 0;
long gt_dist_total_reelG_D[2] = {0, 0};
long gt_derniere_lu_G_D[2] = {0, 0};
bool buttonON =false;
// PID pour robot A
float kp_Origine = 0.0002;
float ki_Origine = 0.00004;
float kp_dec = 0.00002;
float ki_dec = 0.000004;
float kp = kp_Origine;
float ki = ki_Origine;
// PID pour robot B
float kpB_Origine = 0.0004;
float kiB_Origine = 0.00004;
float kpB_dec = 0.00004;
float kiB_dec = 0.000003;
float kpB = kpB_Origine;
float kiB = kiB_Origine;
//variables à mettre dans les variables globales
int g_pince_ouverte[2] = {120, 125};
int g_pince_fermer[2] = {107, 112};
uint16_t dis10bits = 0;
float fDistance = 0;
float distanceSonar = 0;
// variable pour le module suiveur de ligne
int cl1 = 3;
int cl2 = 2;
int cl3 = 1;
int cl4 = -1;
int cl5 = -2;
int cl6 = -3;
// variables pour la lecture des ligne
bool cptLigneRead1;
bool cptLigneRead2;
bool cptLigneRead3;
bool cptLigneRead4;
bool cptLigneRead5;
bool cptLigneRead6;

void readCptLigne() {
    cptLigneRead2 = digitalRead(CPT_LIGNE_2);
    cptLigneRead3 = digitalRead(CPT_LIGNE_3);
    cptLigneRead4 = digitalRead(CPT_LIGNE_4);
    cptLigneRead5 = digitalRead(CPT_LIGNE_5);
    Serial.println(" cpt 2: " + String(cptLigneRead2) + " cpt 3: " + String(cptLigneRead3) 
            + " cpt 4: " + String(cptLigneRead4) + " cpt 5: " + String(cptLigneRead5));
}
bool CheckIntersection() {
    cptLigneRead2 = digitalRead(CPT_LIGNE_2);
    cptLigneRead3 = digitalRead(CPT_LIGNE_3);
    cptLigneRead4 = digitalRead(CPT_LIGNE_4);
    cptLigneRead5 = digitalRead(CPT_LIGNE_5);
    return (!cptLigneRead2 && !cptLigneRead3 && !cptLigneRead4 && !cptLigneRead5);
}

void SuivreLigneReculons() {
    g_vit_motg = -.1;
    g_vit_motd = -.1;
    readCptLigne();
    int erreurLigne = 0;
    if (!cptLigneRead5)
        erreurLigne += cl5;
    if (!cptLigneRead4)
        erreurLigne += cl4;
    if (!cptLigneRead3)
        erreurLigne += cl3;
    if (!cptLigneRead2)
        erreurLigne += cl2;
    // Virage de 90
    if(cptLigneRead5 && cptLigneRead2) {
        float fac = 0.0975;
        g_vit_motg -= erreurLigne*fac/2;
        g_vit_motd += erreurLigne*fac/2;
    }
    /*else if(cptLigneRead5 || cptLigneRead2) {
        float fac = 0.135;
        g_vit_motg += erreurLigne*fac/2;
        g_vit_motd -= erreurLigne*fac/2;
    }*/ else {
        float fac = 0.05;
        g_vit_motg -= erreurLigne*fac/2;
        g_vit_motd += erreurLigne*fac/2;
    }
    MOTOR_SetSpeed(RIGHT, g_vit_motd);
    MOTOR_SetSpeed(LEFT, g_vit_motg );
}






void SuivreLigne() {
    SetOriginalSpeed();
    readCptLigne();
    int erreurLigne = 0;
    if (!cptLigneRead5)
        erreurLigne += cl5;
    if (!cptLigneRead4)
        erreurLigne += cl4;
    if (!cptLigneRead3)
        erreurLigne += cl3;
    if (!cptLigneRead2)
        erreurLigne += cl2;
    // Virage de 90
    if(cptLigneRead5 && cptLigneRead2) {
        float fac = 0.0975;
        g_vit_motg += erreurLigne*fac/2;
        g_vit_motd -= erreurLigne*fac/2;
    }
    /*else if(cptLigneRead5 || cptLigneRead2) {
        float fac = 0.135;
        g_vit_motg += erreurLigne*fac/2;
        g_vit_motd -= erreurLigne*fac/2;
    }*/ else {
        float fac = 0.05;
        g_vit_motg += erreurLigne*fac/2;
        g_vit_motd -= erreurLigne*fac/2;
    }
    MOTOR_SetSpeed(RIGHT, g_vit_motd);
    MOTOR_SetSpeed(LEFT, g_vit_motg);
}
bool IsSteady() {
    return steady;
}
bool SetSteady(bool isSteady) {
    if(isSteady) {
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
    }
    steady = isSteady;
    return steady;
}
void SetOriginalSpeed() {

    g_vit_motd = g_vit_motd_Origin;
    g_vit_motg = g_vit_motg_Origin;
}
//fonctions à mettre avec les autres dans un header
void RelacherBalle()
{
    SERVO_Enable(LEFT);
    SERVO_Enable(RIGHT);

    SERVO_SetAngle(LEFT, g_pince_ouverte[0]);
    SERVO_SetAngle(RIGHT, g_pince_ouverte[1]);
    delay(500);

    SERVO_Disable(LEFT);
    SERVO_Disable(RIGHT);
}
void AttraperBalle()
{

    SERVO_Enable(LEFT);
    SERVO_Enable(RIGHT);

    SERVO_SetAngle(LEFT, g_pince_fermer[0]);
    SERVO_SetAngle(RIGHT, g_pince_fermer[1]);
    delay(500);

    SERVO_Disable(LEFT);
    SERVO_Disable(RIGHT);
}
bool avertisseurSonore()
{
    int detectedSound = analogRead(A0);
    int bruitAmbiant = analogRead(A1);
    Serial.println("bruit detected: " + (String(detectedSound - 30)));
    Serial.println("bruit ambiant: " + String(bruitAmbiant));
    return detectedSound-30 > bruitAmbiant;
}
// fonction pour reset les variables que nous utilisons pour le PID
void reinitialiserVariable()
{
    g_vit_motd = g_vit_motd_Origin;
    g_vit_motg = g_vit_motg_Origin;
    kpB = kpB_Origine;
    kiB = kiB_Origine;
    kp = kp_Origine;
    ki = ki_Origine;
    nbcycle = 0;
    dist_reel_totD = 0;
    dist_reel_totG = 0;
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
    //Serial.println("165: " + String(gt_dist_total_reelG_D[RIGHT]));
    //Serial.println("166: " + String(gt_dist_total_reelG_D[LEFT]));
    // trouve la direction vers la roue tourne pour chacune ( avant positif, arière négatif)
    int direction_droite = g_vit_motd > 0 ? 1 : -1;
    int direction_gauche = g_vit_motg > 0 ? 1 : -1;

    // calcule la différence pour la vitessse pour la roue de droite et gauche
    erreurG = abs(vitesseLuG) - abs(vitesseLuD);
    erreurD = abs(vitesseLuD) - abs(vitesseLuG);
    erreurDistanceD = abs(dist_reel_totD) - abs(dist_reel_totG);
    erreurDistanceG = abs(dist_reel_totG) - abs(dist_reel_totD);

    // dépendant des robots car elles sont composées de matériaux différents,
    // calcule de la compatation pour chacune des roues.

    //comp_d = kp * erreurD + ki * erreurDistanceD; // Robot A
    //comp_g = kp * erreurG + ki * erreurDistanceG;
    comp_d = kpB * erreurD + kiB * erreurDistanceD; //Robot B
    comp_g = kpB * erreurG + kiB * erreurDistanceG;

    // FIN de la dependance pour PID
    g_vit_motd = g_vit_motd - direction_droite * (comp_d / 2);
    g_vit_motg = g_vit_motg - direction_gauche * (comp_g / 2);

    // FIN NOUVEAU
    delay(30);
    MOTOR_SetSpeed(RIGHT, g_vit_motd);
    MOTOR_SetSpeed(LEFT, g_vit_motg);
}

void Avancer(long pulse)
{
    g_vit_motd = 0.22;
    g_vit_motg = 0.22;
    long deceleration = pulse - (pulse * 0.1);
    float acceleration_v;
    bool accel = true;
    float n_pulse_bar = 4200;
    float v_max = (pulse <= CmEnPulse(60)) ? .30 : g_vit_motg_Origin;
    int ki_correction_in_progress = 0;
    bool humainDetecter =false;

    acceleration_v = (v_max - g_vit_motd) / n_pulse_bar;
    Serial.println("DÉBUT: " + String(pulse));
    LigneDroitePID2();
    while (1) //dist_reel_totG < pulse - derniereValeurLuGPulse
    {
        humainDetecter =false;
        distanceSonar = SONAR_GetRange(1);
        if( distanceSonar < 100) 
            humainDetecter =true;
        else
        {
            delay(100);
            distanceSonar = SONAR_GetRange(1);
            if( distanceSonar < 100) 
                humainDetecter =true;
        }
        
        if(humainDetecter)
        {
            /*// acceleration du début
            if (!accel)
            {
                // rien faire car l'acceleration est terminée
            }
            else if (g_vit_motg >= v_max && accel == true)
            {
                accel = false;
            }
            else if (ki_correction_in_progress < 3)
            {
                ki_correction_in_progress++;
                // perte de un tour d'acceleration pour laisser le PID se calibrer
            }
            else
            {
                // augmentation de la vitesse
                g_vit_motg += acceleration_v * gt_derniere_lu_G_D[LEFT];
                g_vit_motd += acceleration_v * gt_derniere_lu_G_D[RIGHT];
                ki_correction_in_progress = 0;
            }

            //decceleration
            if (accel)
            {
            }
            else if (ki_correction_in_progress < 3)
            {
                ki_correction_in_progress++;
                // perte de un tour d'acceleration pour laisser le PID se calibrer
            }
            else if (pulse - dist_reel_totG < n_pulse_bar && g_vit_motd > 0 && g_vit_motd > 0)
            {
                kiB = kiB_dec;
                kpB = kpB_dec;
                ki = ki_dec;
                kp = kp_dec;
                g_vit_motg -= acceleration_v * gt_derniere_lu_G_D[LEFT] * ki_correction_in_progress;
                g_vit_motd -= acceleration_v * gt_derniere_lu_G_D[RIGHT] * ki_correction_in_progress;
                ki_correction_in_progress = 0;
            }
            LigneDroitePID2();*/
            MOTOR_SetSpeed(RIGHT,0.15);
            MOTOR_SetSpeed(LEFT,0.15);
        }
        else
        {   
            MOTOR_SetSpeed(RIGHT,0);
            MOTOR_SetSpeed(LEFT,0);
             g_vit_motd = g_vit_motd_Origin;
            g_vit_motg = g_vit_motg_Origin;
            accel = true;
            reinitialiserVariable();
        }
    }
    Serial.println("FIN: " + String(dist_reel_totG));
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
    reinitialiserVariable();
}


void buttonPress()
{ 
    //buttonON = !buttonON;
    volatile static unsigned long last_interupt_time = 0;
    volatile unsigned long interruptsTime = millis();
    bool ButtonHigh = false;
    int i = 0;
    if(interruptsTime - last_interupt_time > 500)
    {
        last_interupt_time = interruptsTime;
        MOTOR_SetSpeed(RIGHT,0);
        MOTOR_SetSpeed(LEFT,0);
        AX_BuzzerON();
        AX_BuzzerON(5000);
        Serial.println("bontonON");
        
        
        while( i  < 200 ) {
            i++;
            Serial.print(i);
           
            //Serial.println((millis() - interruptsTime));
        }
        i =0;
        ButtonHigh = true;
        
        while(ButtonHigh)
        {  Serial.print(analogRead(A0));
            if(analogRead(A0)> 100) ButtonHigh = false;
        }
        
    }  
     
        AX_BuzzerOFF();
        
        Serial.println("bontonOff");
        buttonON = false;
        

   last_interupt_time = millis();
}
void Virage_1roue(float angle)
{
    long pulse_afaire = angle * 44;
    g_direction = 1;
    if (angle < 0)
    {
        g_direction = -1;
    }

    MOTOR_SetSpeed(RIGHT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(LEFT, g_direction * 0.2);
    while (ENCODER_Read(LEFT) < pulse_afaire)
    {
    }
    MOTOR_SetSpeed(LEFT, 0);
    Serial.println(ENCODER_Read(LEFT));
    reinitialiserVariable();
}
void Virage_1roueDroite(float angle) {
    long pulse_afaire = angle * 44;
    g_direction = 1;
    if (angle < 0)
    {
        g_direction = -1;
    }

    MOTOR_SetSpeed(RIGHT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(RIGHT, g_direction * 0.2);
    while (abs(ENCODER_Read(RIGHT)) < abs(pulse_afaire))
    {
    }
    MOTOR_SetSpeed(RIGHT, 0);
    delay(10);
    reinitialiserVariable();
}
void Virage_2roue(float angle)
{
    int g_direction = 1;
    int roue_maitre = LEFT;
    if (angle < 0)
    {
        g_direction = -1;
        roue_maitre = RIGHT;
    }
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    long pulse_afaire = abs(angle) * 42.6;
    long pulse_distribution = pulse_afaire / 2;

    g_vit_motd = g_direction * -0.14;
    g_vit_motg = g_direction * 0.14;

    Serial.print("abs_encoder: ");
    Serial.println(abs(ENCODER_Read(roue_maitre)));
    Serial.print("pulse_afaire/2: ");
    Serial.println(pulse_distribution);

    LigneDroitePID2();
    while (abs(ENCODER_Read(roue_maitre)) <= pulse_distribution - abs(gt_dist_total_reelG_D[roue_maitre]))
    {
        Serial.println(abs(ENCODER_Read(LEFT)));
        LigneDroitePID2();
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    delay(500);
    Serial.println(pulse_distribution);
    Serial.println(gt_dist_total_reelG_D[roue_maitre]);
    Serial.println(gt_derniere_lu_G_D[roue_maitre]);

    // CORRECTION D'ERREUR
    long pulse_erreur_g = pulse_distribution - abs(gt_dist_total_reelG_D[LEFT]) - abs(gt_derniere_lu_G_D[LEFT]);
    long pulse_erreur_d = pulse_distribution - abs(gt_dist_total_reelG_D[RIGHT]) - abs(gt_derniere_lu_G_D[RIGHT]);
    if (pulse_erreur_d != 0)
    {
        MOTOR_SetSpeed(RIGHT, (roue_maitre == RIGHT ? -.30 : .30));
        pulse_erreur_d = abs(pulse_erreur_d) * 2;
        ENCODER_Reset(RIGHT);
        while (abs(ENCODER_Read(RIGHT)) < pulse_erreur_d)
        {
            Serial.println(abs(ENCODER_Read(RIGHT)));
        }
        MOTOR_SetSpeed(RIGHT, 0);
    }
    // FIN DE correction derreur
    reinitialiserVariable();
}
long CmEnPulse(int distance_cm)
{
    long distancePulse = (3200 / 23.9389 * distance_cm);
    return distancePulse;
}
void testROBOT()
{
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
    for (size_t i = 0; i < 13; i++)
    {
        reinitialiserVariable();
        long distance_degre = instructions[i][0];
        int action = instructions[i][1];
        switch (action)
        {
        case FRONT:
            Serial.println("FRONT");
            Avancer(CmEnPulse(distance_degre));
            delay(10);
            break;

        case TURN:
            Serial.println("TURN");
            instructions[i][0] = -distance_degre;
            Virage_2roue(distance_degre);
            delay(10);
            break;
        }
    }

    // FIN de la loop for
    reinitialiserVariable();
    delay(200);
    Virage_2roue(ang_retour);
    delay(10);
    // FIN RETOUR

    // POUR Y plus grand ou egale au nombre de ligne dans la table
    for (int Y = 12; Y >= 0; Y--)
    {
        reinitialiserVariable();
        int distance_degre = instructions[Y][0];
        int action = instructions[Y][1];
        switch (action)
        {
        case FRONT:
            Serial.println("FRONT");
            Avancer(CmEnPulse(distance_degre));
            delay(10);
            break;

        case TURN:
            Serial.println("TURN");
            Virage_2roue(distance_degre);
            delay(10);
            break;
        }
    }
    // FIN de la loop for du retour
}

int DetectionCouleur(){
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

   if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
  }


 uint16_t r, g, b, c, colorTemp, lux;
  int i = 0;
    while(i < 5){
      tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  i++;

    
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
    }
    
    digitalWrite(PinBLEU, LOW);
    digitalWrite(PinROUGE, LOW);
    digitalWrite(PinJAUNE, LOW);
    digitalWrite(PinVERT, LOW);
    

  if( colorTemp >= 4500 && colorTemp <= 5500){
    digitalWrite(PinROUGE, HIGH);
    return ROUGE;
  }else if( colorTemp >= 3500 && colorTemp <= 4500){
    digitalWrite(PinJAUNE,HIGH);
    return JAUNE;
  }else if( colorTemp >= 5500 && colorTemp <= 6500){
    digitalWrite(PinBLEU, HIGH);
    return BLEU;
  }

}
MFRC522 mfrc522;
void SetRFID(){    
    // Setup RFID
    MFRC522 instanceMFRC(SS_PIN, RST_PIN);
    mfrc522 = instanceMFRC;
    SPI.begin();      // Initiate  SPI bus
    mfrc522.PCD_Init();   // Initiate MFRC522
}

void ResetRFID()
{
SPI.end();
}

int RFID(){ // Create MFRC522 instance.
    // Initiate a serial communication
    Serial.println("Approximate your card to the reader...");
    // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return 0;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return 0;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == Puce1) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Puce1");
    Serial.println();
    return 1;
  }
  else if (content.substring(1) == Puce2)
  {
    Serial.println("Puce2");
    Serial.println();
    return 2;
  }
 else if (content.substring(1) == Puce3)
  {
    Serial.println("Puce3");
    Serial.println();
    return 3;
  }
  else if (content.substring(1) == Carte1)
  {
    Serial.println("Carte1");
    Serial.println();
    return 1;
  }
  else if (content.substring(1) == Carte2)
  {
    Serial.println("Carte2");
    Serial.println();
    return 2;
  }
  else if (content.substring(1) == Carte3)
  {
    Serial.println("Carte3");
    Serial.println();
    return 3;
  }
 else   {
    Serial.println(" Acces Denied");
    return 0;
  }
}