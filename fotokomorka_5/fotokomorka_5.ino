/*
  Program odmierza czas dla zawodników, wykorzystując fotocele
  Zasilanie 12 V stabilizowane
  2x Sensor optyczny WL170-P132
  
  //Start Sequence
  //pixels.Color(B, R, G));
  //digitalRead(sensor1) == 0 - brak odbicia
  //digitalRead(buttonEnter)==1 - wcisniety przycisk
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 6, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define sensor1 9
#define sensor2 10
#define buttonL 5
#define buttonR 4
#define buttonEnter 3
#define buzzer 11

float sensorDistance = 0.2;  //Odległośc pomiędzy sensorami  
int delayTime = 2000 ;
unsigned long sensor1Time, sensor1TimeEnd;
unsigned long sensor2Time, sensor2TimeEnd;
unsigned long personTime, startTime;
int licz = 0;
bool sensor1Active = 0, sensor2Active = 0;
byte sensor1Done = 0;
bool onePerson = 0;
byte cursorX = 0;
byte cursorY = 0;
int mode = 0;

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(buttonEnter, INPUT_PULLUP);
  pinMode(buttonL, INPUT_PULLUP);
  pinMode(buttonR, INPUT_PULLUP);
  pinMode(buzzer,OUTPUT);
  lcd.begin();
  lcd.backlight();
  


}

void loop() {

    //URUCHOMIENIE URZADZENIA - zabezpieczenie
  setDevice();//Menu
  mode = setMode();
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  
  lcd.setCursor(0,1);
  lcd.print("Wcisnij OK   ");
  while(digitalRead(buttonEnter)){}
  lcd.setCursor(0,2);
  lcd.print("Start!       ");
  startTime  = millis(); 
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  pixels.show();
  buz_pip(delayTime);
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  lcd.begin();
  lcd.backlight();
  
  
   while(digitalRead(buttonR))//blad w nazwie buttona
   {
      
     //SENSOR 1   
      if (digitalRead(sensor1) == 1 && sensor1Active == 0 && onePerson == 0)
      {
        sensor1Time = millis() - startTime;
        onePerson = 1;
        sensor1Active = 1;
        pixels.setPixelColor(0, pixels.Color(255, 255, 255));
        pixels.show();
        licz++;
        Serial.print ("S1 = "); Serial.println (sensor1Time);
      }
      else if (digitalRead(sensor1) == 0 && sensor1Active == 1 && onePerson == 1)
      {
        sensor1TimeEnd = millis() - startTime;
        sensor1Active = 0;
        Serial.print ("S1E= "); Serial.println (sensor1TimeEnd);
      }
      //SENSOR 2
    
      if (digitalRead(sensor2) == 1 && onePerson == 1 && sensor2Active == 0)
      {
    
        sensor2Time = millis() - startTime;
        sensor2Active = 1;
        Serial.print ("S2 = "); Serial.println (sensor2Time);
      }
      else if (digitalRead(sensor2) == 0 && onePerson == 1 && sensor1Active == 0 && sensor2Active == 1)
      {
        sensor2TimeEnd = millis() - startTime;
        Serial.print ("S2E= "); Serial.println (sensor2TimeEnd);
        onePerson = 0;
        sensor2Active = 0;
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
        pixels.show();
    
        //Decyzja
        //obiekt mniejszy od odległości między sensorami
        if (sensor1Time < sensor2Time &&  sensor1TimeEnd < sensor2TimeEnd)
        {
          
          /* SERIAL _ DANE
          Serial.print("Zawodnik nr: ");
          Serial.print(licz);
          Serial.print("\tCzas: ");
          Serial.print(sensor1Time / 1000);
          Serial.print("s ");
          Serial.print(sensor1Time % 1000);
          Serial.println("ms ");
          Serial.print("Wielkosc: ");
          Serial.print((sensorDistance / (sensor2Time - sensor1Time) * 1000) * (sensor1TimeEnd - sensor1Time) / 1000 );
          Serial.print("m\t");
          Serial.print("Szybkosc: ");
          Serial.print((sensorDistance / (sensor2Time - sensor1Time) * 3600));
          Serial.print("km/h");
          Serial.println(""); Serial.println("");
    */
          lcd.setCursor(cursorX, cursorY);
          cursorY++;
          if (cursorY == 4) 
          {
            cursorY = 0;
            cursorX += 11;
            if (cursorX > 11)
            {
              cursorX = 0;
              cursorY = 0;
            }
          }
          lcd.print(licz);
          lcd.print(" ");
          lcd.print(sensor1Time / 1000);
          lcd.print(",");
          lcd.print(sensor1Time % 1000);  
          //lcd.print("s");    
          switch(mode)
          {
           case 1:
              startTime  = millis();
              break;
           case 2:
             
              break;
           case 3:
             
              break;
          }
          buz_pip(delayTime);
        }
      }
      pixels.show();
    
    
    
    
    
    }
}
/*



   Serial.println ("S2 - 0");
    Serial.print("sT1: ");
    Serial.print("sT1E: ");
    Serial.print(sensor1TimeEnd);
    Serial.println(" ms");

    Serial.print("sT2: ");
    Serial.print(sensor2Time);
    Serial.print(" ms\t");
    Serial.print("sT2E: ");
    Serial.print(sensor2TimeEnd);
    Serial.println(" ms");

    Serial.print("Zawodnik nr: ");
    Serial.print(licz);
    Serial.print("\tCzas: ");
    Serial.print((millis() - sensor1Time) / 1000);
    Serial.print("s ");
    Serial.print((millis() - sensor1Time) % 1000);
    Serial.println("ms ");
    Serial.print("Szerokosc: ");
    Serial.print((sensorDistance / (sensor1TimeEnd - sensor1Time) * 1000) * (sensor1TimeEnd - sensor1Time) / 1000 );
    Serial.print("m");
    Serial.print("\t");
    Serial.print("Szybkosc: ");
    Serial.print(sensorDistance / (sensor1TimeEnd - sensor1Time) * 1000);
    Serial.print("m/s");
    Serial.print("\t");
    Serial.print("Szybkosc: ");
    Serial.print((sensorDistance / (sensor1TimeEnd - sensor1Time) * 3600));
    Serial.print("km/h");
    Serial.println(""); Serial.println("");

*/

