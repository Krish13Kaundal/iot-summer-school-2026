const int BUZZER_PIN = 3;
const int BUTTONS[] = {4, 5, 6, 7}; // Do, Re, Mi, Fa
const int MODE_BUTTON = 8; // Major/Minor Toggle

// Frequencies for Major and Minor scales
int majorNotes[] = {262, 294, 330, 349, 392}; // Do, Re, Mi, Fa, Sol
int minorNotes[] = {262, 294, 311, 349, 392}; // Minor scale changes Mi to D#/Eb (311Hz)

bool isMajor = true;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }
  pinMode(MODE_BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // Mode Toggle Logic
  if (digitalRead(MODE_BUTTON) == LOW) {
    isMajor = !isMajor;
    Serial.print("Scale changed to: ");
    Serial.println(isMajor ? "Major" : "Minor");
    delay(300); // Debounce
  }

  int pressedCount = 0;
  int activeButtons[4] = {0, 0, 0, 0};

  for (int i = 0; i < 4; i++) {
    if (digitalRead(BUTTONS[i]) == LOW) {
      activeButtons[i] = 1;
      pressedCount++;
    }
  }

  // Play chords or single notes
  if (pressedCount >= 2) {
    // If two or more buttons pressed together, play Sol (index 4)
    if (isMajor) tone(BUZZER_PIN, majorNotes[4]);
    else tone(BUZZER_PIN, minorNotes[4]);
  } 
  else if (pressedCount == 1) {
    for (int i = 0; i < 4; i++) {
      if (activeButtons[i] == 1) {
        if (isMajor) tone(BUZZER_PIN, majorNotes[i]);
        else tone(BUZZER_PIN, minorNotes[i]);
      }
    }
  } 
  else {
    noTone(BUZZER_PIN); // Silence if no button pressed
  }
}
