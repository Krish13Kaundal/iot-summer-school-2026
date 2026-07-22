#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define GREEN_LED 25
#define RED_LED   26
#define POT_PIN   34

Adafruit_BMP085 bmp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float pressureLog[24];
int logIndex = 0;

unsigned long lastRead = 0;
unsigned long lastScreen = 0;
bool page = false;

void setup() {
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  Wire.begin(21,22);

  if(!bmp.begin()){
    Serial.println("BMP180 not found!");
    while(1);
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.println("OLED failed");
    while(1);
  }

  display.clearDisplay();
}

void loop() {

  if(millis()-lastRead>30000 || logIndex==0){

    float pressure = bmp.readPressure()/100.0;
    float temp = bmp.readTemperature();

    int pot = analogRead(POT_PIN);
    float localAltitude = map(pot,0,4095,0,2000);

    float altitude = bmp.readAltitude();
    float seaLevel =
      pressure / pow(1.0 - (localAltitude/44330.0),5.255);

    pressureLog[logIndex%24]=pressure;
    logIndex++;

    float oldest = pressureLog[(logIndex)%24];
    float diff = pressure-oldest;

    String trend="STABLE";

    if(diff>0.5){
      trend="RISING";
      digitalWrite(GREEN_LED,HIGH);
      digitalWrite(RED_LED,LOW);
    }
    else if(diff<-0.5){
      trend="FALLING";
      digitalWrite(RED_LED,HIGH);
      digitalWrite(GREEN_LED,LOW);
    }
    else{
      trend="STABLE";
      digitalWrite(GREEN_LED,HIGH);
      digitalWrite(RED_LED,LOW);
    }

    Serial.println("----------------------------");
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println(" C");

    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");

    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println(" m");

    Serial.print("Sea Level Pressure: ");
    Serial.print(seaLevel);
    Serial.println(" hPa");

    Serial.print("Trend: ");
    Serial.println(trend);

    if(!page){
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,0);
      display.print("Pressure:");
      display.println(pressure);

      display.print("Temp:");
      display.println(temp);

      display.print("Altitude:");
      display.println(altitude);
    }
    else{
      display.clearDisplay();
      display.setTextSize(2);

      if(trend=="RISING")
        display.println("^");
      else if(trend=="FALLING")
        display.println("v");
      else
        display.println("-");

      display.setTextSize(1);
      display.println(trend);
    }

    display.display();

    lastRead=millis();
  }

  if(millis()-lastScreen>5000){
    page=!page;
    lastScreen=millis();
  }
}
