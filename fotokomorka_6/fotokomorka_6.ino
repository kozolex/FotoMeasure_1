#define zielona 9
#define czerwona 8
 
String odebraneDane = ""; //Pusty ciąg odebranych danych
 
void setup() {
  Serial.begin(9600); //Uruchomienie komunikacji
  pinMode(zielona, OUTPUT); //Konfiguracja wyjść
  pinMode(czerwona, OUTPUT);
  
  digitalWrite(zielona, LOW); //Wyłączamy diody
  digitalWrite(czerwona, LOW);
}
 
void loop() {
  if(Serial.available() > 0) { //Czy Arduino odebrano dane
    //Jeśli tak, to odczytujemy je do znaku końca linii i zapisz w zmiennej odebraneDane
    odebraneDane = Serial.readStringUntil('\n'); 
    
    if (odebraneDane == "g") { //Jeśli odebrano słowo "zielona"
      digitalWrite(zielona, HIGH); //To włączamy diodę zieloną
      delay(1000);
      digitalWrite(zielona, LOW); 
    }
    
    if (odebraneDane == "r") { //Jeśli odebrano słowo "czerwona"
      digitalWrite(czerwona, HIGH); //To włączamy diodę czerwoną
      delay(1000);
      digitalWrite(czerwona, LOW); 
    }
    else {
      Serial.println("blad");
    }
}
}

