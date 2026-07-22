#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int trigPin = 9;
const int echoPin = 10;

const int greenLED = 2;
const int yellowLED = 3;
const int redLED = 4;
const int buzzer = 6;

unsigned long lastBeep = 0;

float getDistance() {
  long duration;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop() {

  float sum = 0;

  for (int i = 0; i < 5; i++) {
    sum += getDistance();
    delay(20);
  }

  float distance = sum / 5;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,20);
  display.print(distance,1);
  display.println(" cm");
  display.display();

  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  noTone(buzzer);

  unsigned long now = millis();

  if(distance > 60){
    digitalWrite(greenLED,HIGH);
  }
  else if(distance > 30){
    digitalWrite(yellowLED,HIGH);

    if(now-lastBeep>800){
      tone(buzzer,1000,100);
      lastBeep=now;
    }
  }
  else if(distance > 15){
    digitalWrite(yellowLED,HIGH);
    digitalWrite(redLED,HIGH);

    if(now-lastBeep>300){
      tone(buzzer,1200,100);
      lastBeep=now;
    }
  }
  else{
    digitalWrite(redLED,HIGH);
    tone(buzzer,1500);
  }

  delay(50);
}
