const int LED_PWM_PIN = 9; // Must be a PWM pin (~9)
const int BUTTON_PIN = 2;

int mode = 1; // 1: Slow breathing, 2: Fast pulse, 3: SOS
unsigned long lastDebounceTime = 0;

void setup() {
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Current Mode: 1 (Slow Breathing)");
}

void loop() {
  // Mode selection via push button
  if (digitalRead(BUTTON_PIN) == LOW && (millis() - lastDebounceTime > 300)) {
    mode++;
    if (mode > 3) mode = 1;
    Serial.print("Mode Switched to: ");
    Serial.println(mode == 1 ? "1 (Slow Breathing)" : mode == 2 ? "2 (Fast Pulse)" : "3 (SOS Pattern)");
    lastDebounceTime = millis();
  }

  // Execute breathing profiles based on state
  if (mode == 1) { // Slow breathing (3s cycle -> roughly 6ms per steps)
    for (int i = 0; i <= 255; i++) { analogWrite(LED_PWM_PIN, i); delay(6); }
    for (int i = 255; i >= 0; i--) { analogWrite(LED_PWM_PIN, i); delay(6); }
  } 
  else if (mode == 2) { // Fast pulse (0.5s cycle -> roughly 1ms per steps)
    for (int i = 0; i <= 255; i++) { analogWrite(LED_PWM_PIN, i); delay(1); }
    for (int i = 255; i >= 0; i--) { analogWrite(LED_PWM_PIN, i); delay(1); }
  } 
  else if (mode == 3) { // SOS Pattern (... --- ...)
    // S (...) - Three short flashes
    for(int i=0; i<3; i++) { digitalWrite(LED_PWM_PIN, HIGH); delay(200); digitalWrite(LED_PWM_PIN, LOW); delay(200); }
    delay(200);
    // O (---) - Three long flashes
    for(int i=0; i<3; i++) { digitalWrite(LED_PWM_PIN, HIGH); delay(600); digitalWrite(LED_PWM_PIN, LOW); delay(200); }
    delay(200);
    // S (...) - Three short flashes
    for(int i=0; i<3; i++) { digitalWrite(LED_PWM_PIN, HIGH); delay(200); digitalWrite(LED_PWM_PIN, LOW); delay(200); }
    delay(1000); // Gap before repeating
  }
}
