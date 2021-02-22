//iGeeks

// Power Supplies :-
/*
 - Dump 12V supply from adapter on breadboard.
 - Use Vin & GND to power arduino.
 - Use GND & Vcc to power relay.
 - Use 12V,BGND and 5V(Arduino) to power motor driver.
 - Use Output of B side of motor driver to power motor.
 - use COM & BGND to power Solenoid.(Put 12V in NO of relay.)
 - use 3.3V & AGND to power RFID
 */

// Pin Connections :-
/* 
 -> Motor Driver
 2  : IN1
 3  : ENA
 4  : IN2

-> Relay
 5  : IN1 

-> RFID
 9  : RST
 10 : SDA
 11 : MOSI
 12 : MISO
 13 : SCK
 */
 
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 RFID(SS_PIN, RST_PIN);                // Create MFRC522 instance.

int f=0;
int j=0;
int t=0;
int D=0;                                      //Distance covered by motor
int RF=0;                                     //Output of RFID


void Move()
{
  analogWrite(3,200);
  D=D+7;
  delay(500);                                
  analogWrite(3,0);
  if (D>50)
  {
    if(f==1)
      {
        digitalWrite(2,HIGH);
        digitalWrite(4,LOW);
        f=0;
      }
     else
     {
       digitalWrite(4,HIGH);
       digitalWrite(2,LOW);
       f=1;
     }
    D=0;
   }
}

int Chk(String S)
{
   if (S == "7C E8 5B D3")      
  { return 1; }
  if (S == "F4 AE 68 D3")        
  { return 2; }
  if (S == "D1 C9 4A D3")       
  { return 3; }
  if (S == "EE D6 66 D3")       
  { return 4; }
  if (S == "CD BB 1B D3")        
  { return 5; }
  if (S == "B3 AD 60 D3")        
  { return 6; }
  else   
  {
   Serial.println("No Book Detected");
   delay(1000);
   return 0;
  }
}

void Pop()
 {
   digitalWrite(5,LOW);
   Serial.println("POP!");
   delay(1000);
   digitalWrite(5,HIGH);
   delay(500);
   digitalWrite(5,LOW);
   delay(1000);
   digitalWrite(5,HIGH);
   delay(500);
 }

void setup() 
{
 
Serial.begin(9600);  
SPI.begin();     
RFID.PCD_Init();                              // Initiate MFRC522
RFID.PCD_SetAntennaGain(RFID.RxGain_max);
pinMode(2,OUTPUT);                        
pinMode(3,OUTPUT);                            //Motor Speed
pinMode(4,OUTPUT);                          
pinMode(5,OUTPUT);                            //Controlling Solenoid
Serial.begin(9600);
}

void loop() 
{
  String UID= "";
  digitalWrite(5,HIGH);
  analogWrite(3,0);
  
  if (!RFID.PICC_IsNewCardPresent()) 
  { return;}
  if (!RFID.PICC_ReadCardSerial()) 
  { return; }
  
  for (byte i = 0; i < RFID.uid.size; i++) 
  {
     UID.concat(String(RFID.uid.uidByte[i] < 0x10 ? " 0" : " "));
     UID.concat(String(RFID.uid.uidByte[i], HEX));
  }
  UID.toUpperCase();
  delay(1000);
  Serial.println("UID tag :" + UID);
  if(t==0)
  {
    t=Chk(UID.substring(1));
    if(t!=0)
    {  
      Move();
    }
  }
  else
 {
  j=Chk(UID.substring(1));
 }
  if((t!=0)&&(j!=0))
  {
    if(t==j)
    {Pop();}
    if((t-j)==3)
    {Pop();}
    if((j-t)==3)
    {Pop();}
  }
}
