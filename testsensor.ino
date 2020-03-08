#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>


#include "rgb_lcd.h"

#define DHTPIN 4   // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
rgb_lcd lcd;

byte HEART_SYMBOL = 0; // Nom descriptif pour le symbole
byte heart[8] = { // La répartition des pixels individuels
0b00000,
0b01010,
0b11111,
0b11111,
0b11111,
0b01110,
0b00100,
0b00000
};

const int colorR = 255;
const int colorG = 255;
const int colorB = 0;

int photocellPin = 1; 
void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  pinMode(9,INPUT);
    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.createChar(HEART_SYMBOL, heart);
  lcd.setRGB(colorR, colorG, colorB);
  dht.begin();
  delay(2000);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(3000);
  int valeur = analogRead(photocellPin);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

   //Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    lcd.println("Impossible de lire la sonde DHT!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  int ir = digitalRead(9);

  if (ir == 0) {
  delay(1000);
  lcd.clear();
  lcd.setRGB(0, 240, 0);
  lcd.setCursor(0, 0);
  lcd.print(t);
  lcd.print(" " "\xDF" "C");
  lcd.print(" ");
  lcd.print(h);
  lcd.print(" %");
  lcd.setCursor(0, 1);
  lcd.print("Snake resting");
  lcd.write(HEART_SYMBOL);
  }
  else {

    if (valeur < 100) { 
        lcd.clear();
        lcd.print("Luminosité : ");
          lcd.setCursor(0, 1);
        lcd.print("Sombre");
    }

    if (t <= 23) {
    lcd.setRGB(0,0,255);
    }
    if (23 < t < 23.79) {
    lcd.setRGB(255,165,0);
    lcd.setCursor(0, 8);
    }
    //Si la température est >23 alors le rétroéclairage sera en couleur "Rouge"
    if (t >= 23.80) {
    lcd.setRGB(255,0,0);
    }
  
    
  delay(1000);
  lcd.clear();
  // cursor: colonne 0, ligne 0
  lcd.setCursor(0, 0);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
    // Afficher la température et l'humidité sur le LCD:
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" " "\xDF" "C");
  lcd.setCursor(0, 1);
  lcd.print("Humid: ");
  lcd.print(h);
  lcd.print(" %");
  }

}
