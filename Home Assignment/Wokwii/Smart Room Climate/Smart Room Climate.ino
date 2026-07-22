#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 4
#define DHTTYPE DHT22   // Change to DHT11 if using DHT11

#define RED_LED 25
#define GREEN_LED 26
#define BUZZER 27

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, LOW);

  dht.begin();
  delay(2000);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Error");
    while (true);
  }
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (isnan(temp) || isnan(hum)) {
    display.setCursor(0, 0);
    display.println("Sensor Error");
    display.display();
    delay(2000);
    return;
  }

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temp);
  display.println(" C");

  display.print("Humidity: ");
  display.print(hum);
  display.println(" %");

  if (temp > 38 || hum > 80) {
    display.println("Status: DANGER");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

    tone(BUZZER, 1000);
    delay(500);
    noTone(BUZZER);
  } else {
    display.println("Status: NORMAL");
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }

  display.display();
  delay(2000);
}
