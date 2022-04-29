#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"         

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 2              
#define FANPIN 13          
#define DHTTYPE DHT11  
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void displayStatus(float h, float t, bool fanStat){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.print(" Hum");
  display.setTextSize(2);
  display.print(h);
  display.println(" %");
  display.setTextSize(1);
  display.print("Temp");
  display.setTextSize(2);
  display.print(t);
  display.println(" *C");

  if(fanStat) {
    display.fillRoundRect(0, 42, display.width()-1, 21, 5, WHITE);
    display.setTextSize(2);
    display.setCursor(28, 45);
    display.setTextColor(BLACK);
    display.println("FAN ON");
  }

  display.display(); 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  pinMode(FANPIN, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Anzuchtkasten geht steil boiiiii");
  display.display(); 
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  bool fanStat = false;

  Serial.print(h);
  Serial.print("% - ");
  Serial.print(t);
  Serial.print("°C ");

  if(h > 70) {
    digitalWrite(FANPIN, LOW);
    fanStat = true;
    Serial.print(" - fan on");
  }
  else {
    digitalWrite(FANPIN, HIGH);
    fanStat = false;
  }

  Serial.println("");

  displayStatus(h, t, fanStat);

  delay(1000);
}