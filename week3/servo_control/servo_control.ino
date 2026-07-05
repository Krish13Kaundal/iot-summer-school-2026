#include <Servo.h>

Servo myServo;
const int POT_PIN = A0;
const int SWEEP_BUTTON = 3;
const int SERVO_PIN = 6;

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);
  pinMode(SWEEP_BUTTON, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(SWEEP_BUTTON) == LOW) {
    Serial.println("Executing Full Sweep (0 -> 180 -> 0)");
    for (int pos = 0; pos <= 180; pos++) {
      myServo.write(pos);
      delay(15);
    }
    for (int pos = 180; pos >= 0; pos--) {
      myServo.write(pos);
      delay(15);
    }
    delay(500); // Debounce
  } else {
    int potVal = analogRead(POT_PIN);
    int angle = map(potVal, 0, 1023, 0, 180);
    myServo.write(angle);
    Serial.print("Servo Angle: ");
    Serial.println(angle);
    delay(100);
  }
}
