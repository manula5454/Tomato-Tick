#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int ledPins[6] = {D0, D1, D2, D5, D6, D7};
const int buttonPin = D8;
const int buzzerPin = D9;

unsigned long lastRead = 0;
unsigned long buttonCheck = 0;
const unsigned long interval = 1000;
const unsigned long Binteval = 20;

bool Onsess = false;
bool Onbreak = true;

int studySeconds = 0;
int breakSeconds = 0;
int minutes = 0;
int seconds = 0;

void displayStudytime(){
  minutes = studySeconds / 60;
  seconds = studySeconds % 60;

  display.clearDisplay();

 if (minutes < 10){
    display.setTextSize(3);
    display.setCursor(10, 8);
    display.print("0");
  }
  display.setTextSize(3);
  display.setCursor(35, 8);
  display.print(minutes);
  display.setTextSize(3);
  display.setCursor(53, 8);
  display.print(":");
  if (seconds < 10){
    display.setTextSize(3);
    display.setCursor(70, 8);
    display.print("0");
  }
  display.setTextSize(3);
  display.setCursor(95, 8);
  display.print(seconds);

  display.display();

}

void displayBreaktime(){
  minutes = breakSeconds / 60;
  seconds = breakSeconds % 60;

  display.clearDisplay();

 if (minutes < 10){
    display.setTextSize(3);
    display.setCursor(10, 8);
    display.print("0");
  }
  display.setTextSize(3);
  display.setCursor(35, 8);
  display.print(minutes);
  display.setTextSize(3);
  display.setCursor(53, 8);
  display.print(":");
  if (seconds < 10){
    display.setTextSize(3);
    display.setCursor(70, 8);
    display.print("0");
  }
  display.setTextSize(3);
  display.setCursor(95, 8);
  display.print(seconds);

  display.display();
}

void setup() {
  for (int i = 0; i < 6; i++) pinMode(ledPins[i], OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("PRESS");
  display.setTextSize(2);
  display.setCursor(0, 16);
  display.print("TO START!");
  delay(200);
  display.display();

  while(digitalRead(buttonPin) == HIGH){
    //left empty
  }
  Onsess = true;
  Onbreak = false;
  studySeconds = 1800;
  breakSeconds = 300;

  lastRead = millis();
}

void loop() {
  while (digitalRead(buttonPin) == LOW) {} //wait for button release

  if (digitalRead(buttonPin) == LOW) {
    delay(1000);
    if (digitalRead(buttonPin) == LOW ){ //reset
      while (digitalRead(buttonPin) == LOW) {}
      rp2040.reboot(); //should still work for the RP2350 double check
    }
    delay(50);
    while(digitalRead(buttonPin) == HIGH) { } // pause
    }
    while(digitalRead(buttonPin) == LOW) {}
    delay(50);
    lastRead = millis();
  }

  unsigned long curMills = millis();
  
  if (Onsess && !Onbreak && curMills - lastRead >= interval) {
    lastRead += interval;
    studySeconds--;
    if (studySeconds <= 0){
      Onsess = false;
      Onbreak = true;
      studySeconds = 1800;
      displayBreaktime();
    }
    else{
      displayStudytime();
    }
  }

  if (!Onsess && Onbreak && curMills - lastRead >= interval){
    lastRead += interval;
    breakSeconds--;
    if (breakSeconds<= 0){
      Onsess = true;
      Onbreak = false;
      breakSeconds = 300;
      displayStudytime();
    }
    else{
      displayBreaktime();
    }
  }

  //STUDY SESSION LEDS 
  if (Onsess){

    if(  studySeconds >= 300){
      digitalWrite(ledPins[5], HIGH);
    }
    else {
      digitalWrite(ledPins[5], LOW);
    }
    if( studySeconds >= 600){
      digitalWrite(ledPins[4], HIGH);
    }
    else {
      digitalWrite(ledPins[4], LOW);
    }
    if( studySeconds >= 900){
     digitalWrite(ledPins[3], HIGH);
    }
    else {
      digitalWrite(ledPins[3], LOW);
    }
    if( studySeconds >= 1200){
     digitalWrite(ledPins[2], HIGH);
    }
    else {
      digitalWrite(ledPins[2], LOW);
    }
    if( studySeconds >= 1500){
     digitalWrite(ledPins[1], HIGH);
    }
    else {
      digitalWrite(ledPins[1], LOW);
    }
    if( studySeconds >= 1800){
      digitalWrite(ledPins[0], HIGH);
    }
    else {
     digitalWrite(ledPins[0], LOW);
    }
  }
  else{  

  // BREAK TIME LEDS

    if( Onbreak && breakSeconds >= 60){
     digitalWrite(ledPins[5], HIGH);
    }
    else {
     digitalWrite(ledPins[5], LOW);
    }
    if( breakSeconds >= 120){
      digitalWrite(ledPins[4], HIGH);
    }
    else {
     digitalWrite(ledPins[4], LOW);
    }
    if( breakSeconds >= 180){
     digitalWrite(ledPins[3], HIGH);
    }
    else {
     digitalWrite(ledPins[3], LOW);
    }
    if( breakSeconds >= 240){
      digitalWrite(ledPins[2], HIGH);
    }
    else {
      digitalWrite(ledPins[2], LOW);
    }
    if( breakSeconds >= 300){           // BOTH LEDS LIGHT UP WHEN FINISHED
      digitalWrite(ledPins[1], HIGH);
      digitalWrite(ledPins[0], HIGH);
    }
    else {
      digitalWrite(ledPins[1], LOW);
      digitalWrite(ledPins[0], LOW);
    }
  }


}
