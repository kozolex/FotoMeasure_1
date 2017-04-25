/*
  Program odmierza czas dla zawodników, wykorzystując fotocele
  Zasilanie 12 V stabilizowane
  2x Sensor optyczny WL170-P132
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 6, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define sensor1 9
#define sensor2 10
#define buzzer 11
#define button 4

float sensorDistance = 0.2;
int delayTime = 1000 ;
unsigned long sensor1Time, sensor1TimeEnd;
unsigned long sensor2Time, sensor2TimeEnd;
unsigned long personTime, startTime;
int licz = 0;
bool sensor1Active = 0, sensor2Active = 0;
byte sensor1Done = 0;
bool onePerson = 0;
byte cursorX = 0;
byte cursorY = 0;
bool typeMeasure = 0; //0 odleglosc wielu zawodników

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
/*
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("FOTOKOMORKA v1.0");
*/
  menu();
  //startSeq(100);
  startSeq2(100);
}

//############# LOOP ###############
void loop() {
  //SENSOR 1
  if (digitalRead(sensor1) == 1 && sensor1Active == 0 && onePerson == 0)
  {
    sensor1Time = millis() - startTime;
    onePerson = 1;
    sensor1Active = 1;
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));
    digitalWrite(buzzer, 1);
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
    digitalWrite(buzzer, 0);
    pixels.show();

    //Decyzja
    //obiekt mniejszy od odległości między sensorami
    if (sensor1Time < sensor2Time &&  sensor1TimeEnd < sensor2TimeEnd)
    {
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
    }
  }
  lcd.setCursor(cursorX, cursorY);
  //lcd.print(licz);
  lcd.print("  ");
  lcd.print((millis() - startTime) / 1000);
  lcd.print(",");
  lcd.print((millis() - startTime) % 1000);


  pixels.show();
}
//############# START ###############
void startSeq(int delayTime)
{
  //Start Sequence
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();
  Serial.println("3");
  delay(delayTime);
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
  Serial.println("2");
  delay(delayTime);
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
  Serial.println("1");
  delay(delayTime);
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  pixels.show();
  delay(delayTime);
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  Serial.println("START");
  startTime  = millis();
  digitalWrite(buzzer, 1);
  delay(300);
  digitalWrite(buzzer, 0);
}



//############# START 2 ###############
void startSeq2(byte timeDelay)
{
  lcd.begin();
  lcd.print("CZEKAM NA START");
  while (1)
    if (digitalRead(sensor2) == 1 || digitalRead(sensor1) == 1)
    {
      startTime  = millis();
      pixels.setPixelColor(0, pixels.Color(255, 255, 255));
      pixels.show();
      digitalWrite(buzzer, 1);
      delay(timeDelay);
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();
      digitalWrite(buzzer, 0);
      lcd.begin();
      break;
    }
}
//############# MENU ###############
void menu()
{
  lcd.begin();
  lcd.print("        MENU        ");
  lcd.setCursor(0, 1);
  lcd.print("Sens dist:");
  lcd.print(sensorDistance * 100);
  lcd.print(" cm");
  // Wybor odleglosci
  startTime = (millis());
  while (1)
  {
    if (digitalRead(button) == 0)
    {
      digitalWrite(buzzer, 1);
      lcd.setCursor(10, 1);
      lcd.print(sensorDistance * 100);
      lcd.print(" cm");
      sensorDistance += 0.1;
      delay(200);
      digitalWrite(buzzer, 0);
    }
    if (sensorDistance == 0.5) sensorDistance = 0.1;
    if (millis() - startTime > 5000)
    {
      lcd.begin();
      break;
    }
    lcd.setCursor(0, 0);
    lcd.print(4 - ((millis() - startTime) / 1000));
  }
  while (1)
  {
    if (digitalRead(button) == 0)
    {
      digitalWrite(buzzer, 1);
      lcd.setCursor(10, 1);
      lcd.print(sensorDistance * 100);
      lcd.print(" cm");
      sensorDistance += 0.1;
      delay(200);
      digitalWrite(buzzer, 0);
    }
    if (sensorDistance == 0.5) sensorDistance = 0.1;
    if (millis() - startTime > 5000)
    {
      lcd.begin();
      break;
    }
    lcd.setCursor(0, 0);
    lcd.print(4 - ((millis() - startTime) / 1000));
  }
}


