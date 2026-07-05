#include "DHT.h"

#define DHTPIN 2     // Pin connected to DHT11
#define DHTTYPE DHT11

const int RED_LED = 4;
const int GREEN_LED = 5;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  // CSV Header
  Serial.println("timestamp_ms,temp_C,temp_F,humidity_pct");
}

void loop() {
  delay(2000); // Read every 2 seconds

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true); // Fahrenheit

  if (isnan(h) || isnan(t) || isnan(f)) {
    return;
  }

  // Print in CSV format
  Serial.print(millis()); Serial.print(",");
  Serial.print(t); Serial.print(",");
  Serial.print(f); Serial.print(",");
  Serial.println(h);

  // Trigger conditions
  if (t > 35.0 || h > 80.0) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
}
