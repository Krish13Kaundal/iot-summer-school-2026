/*
 * STATE TRANSITION DIAGRAM:
 * * [ IDLE ] ------(Insert Coin)-----> [ COIN_INSERTED ]
 * ^                                      |      |
 * |-------------(Cancel)-----------------|      | (Select Item)
 * |                                             v
 * [ IDLE ] <---(Auto Finish)--- [ DISPENSING ] <-- [ ITEM_SELECTED ]
 */

enum State { IDLE, COIN_INSERTED, ITEM_SELECTED, DISPENSING };
State currentState = IDLE;

const int BTN_COIN = 4;
const int BTN_SELECT = 5;
const int BTN_CANCEL = 6;

const int LED_IDLE = 8;
const int LED_COIN = 9;
const int LED_DISPENSE = 10;

void setup() {
  pinMode(BTN_COIN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_CANCEL, INPUT_PULLUP);

  pinMode(LED_IDLE, OUTPUT);
  pinMode(LED_COIN, OUTPUT);
  pinMode(LED_DISPENSE, OUTPUT);

  Serial.begin(9600);
  Serial.println("System Initialized: State -> IDLE");
}

void loop() {
  // Handle state indicator LEDs
  digitalWrite(LED_IDLE, currentState == IDLE ? HIGH : LOW);
  digitalWrite(LED_COIN, currentState == COIN_INSERTED ? HIGH : LOW);
  digitalWrite(LED_DISPENSE, currentState == DISPENSING ? HIGH : LOW);

  switch (currentState) {
    case IDLE:
      if (digitalRead(BTN_COIN) == LOW) {
        delay(200); // Debounce
        currentState = COIN_INSERTED;
        Serial.println("TRANSITION: IDLE -> COIN_INSERTED");
      }
      break;

    case COIN_INSERTED:
      if (digitalRead(BTN_SELECT) == LOW) {
        delay(200);
        currentState = ITEM_SELECTED;
        Serial.println("TRANSITION: COIN_INSERTED -> ITEM_SELECTED");
      } 
      else if (digitalRead(BTN_CANCEL) == LOW) {
        delay(200);
        currentState = IDLE;
        Serial.println("TRANSITION: COIN_INSERTED -> IDLE (Refunded)");
      }
      break;

    case ITEM_SELECTED:
      // Instantly move to dispensing logic
      currentState = DISPENSING;
      Serial.println("TRANSITION: ITEM_SELECTED -> DISPENSING");
      break;

    case DISPENSING:
      Serial.println("Processing... Vending Item.");
      delay(3000); // Simulate item dropping down
      currentState = IDLE;
      Serial.println("TRANSITION: DISPENSING -> IDLE (Transaction Complete)");
      break;
  }
}
