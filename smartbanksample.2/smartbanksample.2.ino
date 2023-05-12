#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

#define inSensor 2
#define outSensor 3
 
int inStatus;
int outStatus;
 
int countin = 0;
int countout = 0;
 
int in;
int out;
int now;
 

 
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
String UID = "10 12 16 1C";
byte lock = 0;

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);


void setup()
{
  lcd.begin();
  lcd.backlight();
  delay(1000);
  pinMode(inSensor, INPUT);
  pinMode(outSensor, INPUT);
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Visitor Counter");
  delay(2000);
  Serial.begin(9600);
  servo.write(70);
  lcd.begin();
  lcd.backlight();
  servo.attach(6);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() 
{
  inStatus =  digitalRead(inSensor);
  outStatus = digitalRead(outSensor);
  if (inStatus == 0)
  {
    in = countin++;
  }
 
  if (outStatus == 0)
  {
    out = countout++;
  }
 
  now = in - out;
 
  if (now <= 0)
  {
    lcd.clear();
    lcd.print("Nobody In Room");
    lcd.setCursor(0, 1);
    lcd.print("Light Off");
    delay(500);
  }
  else
  {
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("Person In Room");

    lcd.setCursor(0,1);
    lcd.print(now);
    delay(500);
  }
{
  lcd.setCursor(4, 0);
  lcd.print("Welcome!");
  lcd.setCursor(1, 1);
  lcd.print("Put your card");

  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scanning");
  Serial.print("NUID tag is :");
  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    lcd.print(".");
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(300);
  }
  ID.toUpperCase();
  Serial.print(ID);
  if (ID.substring(1) == UID && lock == 0 ) {
    servo.write(70);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Door is locked");
    delay(1500);
    lcd.clear();
    lock = 1;
  } else if (ID.substring(1) == UID && lock == 1 ) {
    servo.write(160);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Door is open");
    delay(1500);
    lcd.clear();
    lock = 0;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wrong card!");
    delay(1500);
    lcd.clear();
  }
}}
