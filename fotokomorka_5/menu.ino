void buz_pip(int time)
{
      digitalWrite(buzzer,1);
      delay(time);
      digitalWrite(buzzer,0);
      delay(time);
}


void setDevice()
{
  lcd.setCursor(0,0);
  lcd.print("Fotokomorka ver.1");
  lcd.setCursor(0,1);
  lcd.print("Ustaw i wcisnij OK!");
  do{
    
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));  
    pixels.show();
    if (digitalRead(sensor1) == 1 && digitalRead(sensor2) == 1)
    {
      digitalWrite(buzzer,1);
    }
    else if (digitalRead(sensor1) == 1 ||digitalRead(sensor2) == 1) 
    {
    buz_pip(100);
    }
    else digitalWrite(buzzer,0); 

  }  while(digitalRead(sensor1) == 1 ||digitalRead(sensor2) == 1 ||digitalRead(buttonEnter) == 1);//
  digitalWrite(buzzer,0);
  
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
    lcd.begin();
  lcd.backlight();
}


int setMode()
{
  lcd.setCursor(0,0);
  lcd.print(" TRYB  1 / 2 / 3   ");
  lcd.setCursor(0,1);
  lcd.print("1 BIEG 1os    ");
  lcd.setCursor(0,2);
  lcd.print("2 BIEG wiele osob  ");
  lcd.setCursor(0,3);
  lcd.print("3 INTERWAL         ");
  buz_pip(100);
  while(1)
  {

    if (digitalRead(buttonEnter) == 0) 
    {
      lcd.begin();
      lcd.setCursor(0,0);
      lcd.print("INTERWAL         ");
      delayTime = 1500;
      return 3; 
      break;
    }
    if (digitalRead(buttonL) == 0)
       {
      lcd.begin();
      lcd.setCursor(0,0);
      lcd.print("BIEG wiele os        ");
      delayTime = 75;
      return 2; 
      break;
    }
    if (digitalRead(buttonR) == 0)
        {
      lcd.begin();
      lcd.setCursor(0,0);
      lcd.print("BIEG 1 os         ");
      delayTime = 1000;
      return 1; 
      break;
    }
  }
  buz_pip(100);

      
    lcd.begin(); 
}
