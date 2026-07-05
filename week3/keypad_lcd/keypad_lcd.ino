#include <LiquidCrystal.h>
#include <Keypad.h>

// Initialize LCD pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3}; 
byte colPins[COLS] = {6, 7, 8, 9}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

const String SECRET_PIN = "2026";
String inputPIN = "";
int wrongAttempts = 0;

const int GREEN_LED = 10;
const int RED_LED = 13;
const int BUZZER = A4;

void setup(){
  lcd.begin(16, 2);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  resetSystem();
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    if (customKey == '#') { // Press '#' to clear or submit empty
      inputPIN = "";
      resetSystem();
    } 
    else {
      inputPIN += customKey;
      lcd.setCursor(inputPIN.length() - 1, 1);
      lcd.print('*'); // Obfuscate view
      
      if (inputPIN.length() == 4) {
        delay(300);
        lcd.clear();
        if (inputPIN == SECRET_PIN) {
          lcd.print("ACCESS GRANTED");
          digitalWrite(GREEN_LED, HIGH);
          delay(3000);
          wrongAttempts = 0;
          resetSystem();
        } else {
          lcd.print("ACCESS DENIED");
          digitalWrite(RED_LED, HIGH);
          tone(BUZZER, 800, 1000);
          delay(2000);
          wrongAttempts++;
          
          if (wrongAttempts >= 3) {
            lcd.clear();
            lcd.print("LOCKED OUT!");
            for(int i = 10; i > 0; i--) {
              lcd.setCursor(0,1);
              lcd.print("Wait: "); lcd.print(i); lcd.print("s ");
              delay(1000);
            }
            wrongAttempts = 0;
          }
          resetSystem();
        }
      }
    }
  }
}

void resetSystem() {
  inputPIN = "";
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  noTone(BUZZER);
  lcd.clear();
  lcd.print("ENTER PIN:");
}
