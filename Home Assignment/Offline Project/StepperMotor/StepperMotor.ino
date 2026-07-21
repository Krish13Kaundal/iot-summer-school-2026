#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Stepper.h>

// --- OLED Setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Stepper Motor Setup ---
const int STEPS_PER_REV = 2048; 
Stepper stepper(STEPS_PER_REV, 8, 10, 9, 11);

// --- Pin Definitions ---
const int POT_PIN = A0;
const int LED_PIN = 13;

// --- State Variables ---
long currentSteps = 0;
int currentAngle = 0;
int targetAngle = 0;
String directionStr = "STOP";

void setup() {
  Serial.begin(9600); // Initialize Serial communication
  pinMode(LED_PIN, OUTPUT);

  stepper.setSpeed(10);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(F(" Precision Positioner"));
  display.println(F(" Serial Mode Ready"));
  display.display();
  delay(1000);

  Serial.println(F("--- Serial Controls ---"));
  Serial.println(F("Send 'R' or 'r' -> Rotate CW (45 deg)"));
  Serial.println(F("Send 'L' or 'l' -> Rotate CCW (45 deg)"));
  Serial.println(F("Send 'H' or 'h' -> Set HOME"));
}

void loop() {
  // 1. Read Potentiometer
  int potValue = analogRead(POT_PIN);
  targetAngle = map(potValue, 0, 1023, 0, 360);

  // 2. Read Serial Commands
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == 'R' || cmd == 'r') { // Clockwise
      digitalWrite(LED_PIN, HIGH);
      directionStr = "CW";
      stepper.step(256);
      currentSteps += 256;
      digitalWrite(LED_PIN, LOW);
    } 
    else if (cmd == 'L' || cmd == 'l') { // Counter-Clockwise
      digitalWrite(LED_PIN, HIGH);
      directionStr = "CCW";
      stepper.step(-256);
      currentSteps -= 256;
      digitalWrite(LED_PIN, LOW);
    } 
    else if (cmd == 'H' || cmd == 'h') { // Set Home
      currentSteps = 0;
      directionStr = "HOME SET";
    }
  } else {
    directionStr = "STOP";
  }

  // 3. Calculate Normalized Angle
  long modSteps = currentSteps % STEPS_PER_REV;
  if (modSteps < 0) modSteps += STEPS_PER_REV;
  currentAngle = (modSteps * 360) / STEPS_PER_REV;

  // 4. Update OLED Display
  updateDisplay();
  delay(50);
}

void updateDisplay() {
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("Current: "));
  display.print(currentAngle);
  display.write(247);

  display.setCursor(0, 16);
  display.print(F("Target:  "));
  display.print(targetAngle);
  display.write(247);

  display.setCursor(0, 32);
  display.print(F("Dir:     "));
  display.print(directionStr);

  display.setCursor(0, 48);
  display.print(F("Steps:   "));
  display.print(currentSteps);

  display.display();
}
