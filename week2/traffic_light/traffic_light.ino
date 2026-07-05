const int RED_LED = 11;
const int YELLOW_LED = 12;
const int GREEN_LED = 13;
const int BUTTON_PIN = 7;

unsigned long previousMillis = 0;
int state = 0; // 0: RED, 1: YELLOW, 2: GREEN

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // Check if pedestrian button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.print("["); Serial.print(millis()); Serial.println("] PEDESTRIAN CROSSING: Forcing RED");
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(8000); // Hold RED for 8 seconds
    previousMillis = millis();
    state = 1; // Go to YELLOW next
  }

  unsigned long currentMillis = millis();

  if (state == 0) { // RED State (5 seconds)
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    if (currentMillis - previousMillis >= 5000) {
      Serial.print("["); Serial.print(millis()); Serial.println("] State Change: RED -> YELLOW");
      previousMillis = currentMillis;
      state = 1;
    }
  } 
  else if (state == 1) { // YELLOW State (2 seconds)
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    if (currentMillis - previousMillis >= 2000) {
      Serial.print("["); Serial.print(millis()); Serial.println("] State Change: YELLOW -> GREEN");
      previousMillis = currentMillis;
      state = 2;
    }
  } 
  else if (state == 2) { // GREEN State (4 seconds)
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    if (currentMillis - previousMillis >= 4000) {
      Serial.print("["); Serial.print(millis()); Serial.println("] State Change: GREEN -> RED");
      previousMillis = currentMillis;
      state = 0;
    }
  }
}
