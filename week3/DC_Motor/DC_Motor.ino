const int ENABLE_PIN = 9; // PWM pin for Speed Control (Pin 1 of L293D)
const int INPUT1 = 7;     // Direction Control Pin (Pin 2 of L293D)
const int INPUT2 = 8;     // Direction Control Pin (Pin 7 of L293D)
const int BTN_DIR = 2;
const int BTN_STOP = 3;
const int POT_PIN = A0;

bool isForward = true;
bool isRunning = true;

void setup() {
  Serial.begin(9600);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(INPUT1, OUTPUT);
  pinMode(INPUT2, OUTPUT);
  pinMode(BTN_DIR, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BTN_STOP) == LOW) {
    isRunning = !isRunning;
    delay(300); // Debounce
  }

  if (digitalRead(BTN_DIR) == LOW) {
    isForward = !isForward;
    delay(300); // Debounce
  }

  int potValue = analogRead(POT_PIN);
  int pwmSpeed = map(potValue, 0, 1023, 0, 255);
  int speedPct = map(pwmSpeed, 0, 255, 0, 100);

  if (isRunning) {
    analogWrite(ENABLE_PIN, pwmSpeed); // Control speed via PWM
    if (isForward) {
      digitalWrite(INPUT1, HIGH);
      digitalWrite(INPUT2, LOW);
    } else {
      digitalWrite(INPUT1, LOW);
      digitalWrite(INPUT2, HIGH);
    }
  } else {
    analogWrite(ENABLE_PIN, 0); // Stop motor
  }

  Serial.print("State: "); Serial.print(isRunning ? "RUNNING" : "STOPPED");
  Serial.print(" | Direction: "); Serial.print(isForward ? "FORWARD" : "REVERSE");
  Serial.print(" | Speed: "); Serial.print(speedPct); Serial.println("%");
  delay(300);
}
