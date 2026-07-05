#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Initialize I2C LCD (0x27 address, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Keypad pins configured away from I2C pins
byte rowPins[ROWS] = {A0, A1, A2, A3}; 
byte colPins[COLS] = {6, 7, 8, 9}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

const String SECRET_PIN = "2026";
String inputPIN = "";
int wrongAttempts = 0;

const int BUZZER = 11; 

void setup(){
  lcd.init();         
  lcd.backlight();    
  pinMode(BUZZER, OUTPUT);
  resetSystem();
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    if (customKey == '#') { 
      inputPIN = "";
      resetSystem();
    } 
    else {
      inputPIN += customKey;
      lcd.setCursor(inputPIN.length() - 1, 1);
      lcd.print('*'); 
      
      if (inputPIN.length() == 4) {
        delay(300);
        lcd.clear();
        if (inputPIN == SECRET_PIN) {
          lcd.print("ACCESS GRANTED");
          delay(3000);
          wrongAttempts = 0;
          resetSystem();
        } else {
          lcd.print("ACCESS DENIED");
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
  noTone(BUZZER);
  lcd.clear();
  lcd.print("ENTER PIN:");
}
