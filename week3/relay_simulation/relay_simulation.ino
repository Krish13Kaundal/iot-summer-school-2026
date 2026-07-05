#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

const int RELAY_PIN = 8; // Simulates AC Appliance
const int OVERRIDE_BTN = 3;

DHT dht(DHTPIN, DHTTYPE);
bool relayState = false;
bool manualOverride = false;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(OVERRIDE_BTN, INPUT_PULLUP);
}

void loop() {
  float temp = dht.readTemperature();
  if (isnan(temp)) return;

  if (digitalRead(OVERRIDE_BTN) == LOW) {
    manualOverride = !manualOverride;
    relayState = !relayState;
    Serial.print("MANUAL OVERRIDE TRIGGERED. Relay state forced: ");
    Serial.println(relayState ? "ON" : "OFF");
    delay(400); // Debounce
  }

  if (!manualOverride) {
    // Automatic control logic with Hysteresis
    if (temp > 32.0 && !relayState) {
      relayState = true;
      Serial.print("AUTOMATIC ON: Temp = "); Serial.print(temp); Serial.println(" C -> AC turned ON");
    } 
    else if (temp < 28.0 && relayState) {
      relayState = false;
      Serial.print("AUTOMATIC OFF: Temp = "); Serial.print(temp); Serial.println(" C -> AC turned OFF");
    }
  }

  digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);
  delay(2000);
}
