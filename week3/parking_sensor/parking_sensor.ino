const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int RED_LED = 5;
const int YELLOW_LED = 6;
const int GREEN_LED = 7;
const int BUZZER_PIN = 3;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Trigger Ultrasonic Sensor pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo duration
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate distance using the given formula
  float distance = (duration * 0.034) / 2;

  // Condition 1: Distance > 50cm (SAFE)
  if (distance > 50) {
    Serial.println("SAFE");
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER_PIN);
    delay(100); // Small stability delay
  } 
  // Condition 2: Distance 20cm to 50cm (Yellow LED + 500ms Beep)
  else if (distance >= 20 && distance <= 50) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    
    tone(BUZZER_PIN, 1000);
    delay(500);
    noTone(BUZZER_PIN);
    delay(500);
  } 
  // Condition 3: Distance 10cm to 20cm (Red LED + 200ms Beep)
  else if (distance >= 10 && distance < 20) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    
    tone(BUZZER_PIN, 1500);
    delay(200);
    noTone(BUZZER_PIN);
    delay(200);
  } 
  // Condition 4: Distance < 10cm (Rapid Flash + Continuous Buzzer)
  else if (distance < 10) {
    tone(BUZZER_PIN, 2000); // Continuous sound
    
    // Rapid flashing using short delays
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    delay(100);
    
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    delay(100);
  }
}
