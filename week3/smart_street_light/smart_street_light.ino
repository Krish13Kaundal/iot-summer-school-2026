const int LDR_PIN = A0;
const int PIR_PIN = 3;
const int LED_PWM_PIN = 6;

const int LDR_THRESHOLD = 400; // Adjust based on darkness level
unsigned long motionMillis = 0;
bool motionActive = false;

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PWM_PIN, OUTPUT);
  Serial.println("[00:00:00] EVENT: System Boot Up");
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  int pirState = digitalRead(PIR_PIN);

  if (ldrValue < LDR_THRESHOLD) { // Dark (Night Mode)
    if (pirState == HIGH) {
      if (!motionActive) {
        Serial.print("["); Serial.print(millis()/1000); Serial.println("] EVENT: Motion Detected! Full Brightness");
        motionActive = true;
      }
      analogWrite(LED_PWM_PIN, 255); // 100% Brightness
      motionMillis = millis();
    } else {
      if (motionActive && (millis() - motionMillis > 30000)) {
        Serial.print("["); Serial.print(millis()/1000); Serial.println("] EVENT: Timeout. Dimming to 20%");
        motionActive = false;
      }
      if (!motionActive) {
        analogWrite(LED_PWM_PIN, 51); // 20% of 255
      }
    }
  } else { // Daylight Mode
    analogWrite(LED_PWM_PIN, 0); // Off
    if (motionActive) motionActive = false;
  }
  delay(500);
}
