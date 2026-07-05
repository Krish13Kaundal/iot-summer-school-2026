#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int LDR_PIN = A0;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  delay(5000); // Log every 5 seconds

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // LDR Conversion
  int ldrRaw = analogRead(LDR_PIN);
  int lightPct = map(ldrRaw, 0, 1023, 0, 100);
  String lightDesc = (lightPct > 70) ? "Bright" : (lightPct > 30) ? "Normal" : "Dark";

  // Ultrasonic distance
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = (duration * 0.034) / 2;

  Serial.println("=== SENSOR LOG ===");
  Serial.print("Time     : "); Serial.print(millis()); Serial.println(" ms");
  Serial.print("Temp     : "); Serial.print(t, 1); Serial.print(" C | Humidity: "); Serial.print(h, 0); Serial.println("%");
  Serial.print("Light    : "); Serial.print(lightPct); Serial.print("% ("); Serial.print(lightDesc); Serial.println(")");
  Serial.print("Distance : "); Serial.print(distance); Serial.println(" cm");
  Serial.println("==================");
}
