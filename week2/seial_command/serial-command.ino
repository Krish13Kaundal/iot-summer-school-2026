const int LED_PIN = 13;
int blinkCounter = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Serial Command Interface Ready.");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove trailing whitespaces

    if (command == "LED_ON") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("SUCCESS: LED is ON");
    } 
    else if (command == "LED_OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("SUCCESS: LED is OFF");
    } 
    else if (command.startsWith("BLINK_")) {
      char numChar = command.charAt(6);
      if (numChar >= '1' && numChar <= '9') {
        int x = numChar - '0';
        Serial.print("SUCCESS: Blinking LED "); Serial.print(x); Serial.println(" times");
        for (int i = 0; i < x; i++) {
          digitalWrite(LED_PIN, HIGH); delay(200);
          digitalWrite(LED_PIN, LOW); delay(200);
          blinkCounter++;
        }
      } else {
        Serial.println("ERROR: Invalid Blink Count");
      }
    } 
    else if (command == "STATUS") {
      Serial.print("PIN 13 State: ");
      Serial.println(digitalRead(LED_PIN) ? "HIGH" : "LOW");
      Serial.print("Total Blinks Executed: ");
      Serial.println(blinkCounter);
    } 
    else if (command == "RESET") {
      blinkCounter = 0;
      Serial.println("SUCCESS: Blink counter reset to 0");
    } 
    else {
      Serial.println("ERROR: Unknown command");
    }
  }
}
