#include <Arduino.h>
#include <LibRobus.h>
#include <musique.h>
#include <RobusDarthGenius.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

void setup()
{
    // BOILER PLATE SETUP
    BoardInit();// Initiate a serial communication
    SPI.begin();      // Initiate  SPI bus
    mfrc522.PCD_Init();   // Initiate MFRC522
    Serial.println("Approximate your card to the reader...");
    Serial.begin(9600);
    // Allumer les PIN pour la detection de couleur
    pinMode(PinBLEU, OUTPUT);
    pinMode(PinJAUNE, OUTPUT);
    pinMode(PinROUGE, OUTPUT);
    pinMode(PinVERT, OUTPUT);

}

/*
void prendreBallon()
{
    int couleur = 0;
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
    couleur = DetectionCouleur();
    reinitialiserVariable();
    Avancer(CmEnPulse(150), false);
    reinitialiserVariable();
    AttraperBalle();
    delay(500);

    if (couleur == JAUNE) //jaune
    {
        Avancer(CmEnPulse(84), false);
        reinitialiserVariable();
        Virage_2roue(93);
        reinitialiserVariable();
        Avancer(CmEnPulse(25), false);
        reinitialiserVariable();
    }
    else if (couleur == BLEU ) //bleu
    {
        Avancer(CmEnPulse(163), false);
        reinitialiserVariable();
        Virage_2roue(-90);
        reinitialiserVariable();
        Avancer(CmEnPulse(25), false);
        reinitialiserVariable();
    }
    else if (couleur == ROUGE) //rouge
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
    delay(1000);
    prendreBallon();
    
// Serial.println("dans fonction loop");
// reinitialiserVariable();
// distanceSonar= SONAR_GetRange(1);
// delay(500);
// Serial.println(distanceSonar);
// float distanceSonar = 0;
// distanceSonar = SONAR_GetRange(1);
// Avancer(CmEnPulse(200), true);
// Avancer(CmEnPulse(200), true);
// Avancer(CmEnPulse(75), true);
// delay(500);
// exit(0);

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
}*/
 
 
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
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
    digitalWrite(PinROUGE,HIGH);
    delay(3000);
  

  }
  else if (content.substring(1) == Puce2)
  {
    Serial.println("Puce2");
    Serial.println();
    digitalWrite(PinBLEU,HIGH);
    delay(3000);
  }
 else if (content.substring(1) == Puce3)
  {
    Serial.println("Puce3");
    Serial.println();
    digitalWrite(PinJAUNE,HIGH);
    delay(3000);
  }
  else if (content.substring(1) == Puce4)
  {
    Serial.println("Puce4");
    Serial.println();
    digitalWrite(PinVERT,HIGH);
    delay(3000);
  }
 else   {
    Serial.println(" Acces Denied");
    delay(3000);
  }
  digitalWrite(PinROUGE,LOW);
  digitalWrite(PinBLEU,LOW);
  digitalWrite(PinVERT,LOW);
  digitalWrite(PinJAUNE,LOW);
}