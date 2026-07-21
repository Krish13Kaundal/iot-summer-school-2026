#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define RELAY1 26
#define RELAY2 27
#define BUZZER 25

bool light1 = false;
bool light2 = false;

void beep() {
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}

void updateRelays() {
  digitalWrite(RELAY1, light1 ? LOW : HIGH);
  digitalWrite(RELAY2, light2 ? LOW : HIGH);
}

void sendStatus() {
  String status = "L1:";
  status += (light1 ? "ON " : "OFF ");
  status += "L2:";
  status += (light2 ? "ON" : "OFF");

  Serial.println(status);
  SerialBT.println(status);
}

void setup() {

  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

  SerialBT.begin("IIT_IoT_HomeCtrl");

  Serial.println("Bluetooth Started");
  Serial.println("Connect to IIT_IoT_HomeCtrl");
}

void loop() {

  if (SerialBT.available()) {

    char cmd = SerialBT.read();

    switch (cmd) {

      case '1':
        light1 = true;
        break;

      case '2':
        light1 = false;
        break;

      case '3':
        light2 = true;
        break;

      case '4':
        light2 = false;
        break;

      case '5':
        light1 = true;
        light2 = true;
        break;

      case '6':
        light1 = false;
        light2 = false;
        break;

      case '?':
        sendStatus();
        return;

      default:
        SerialBT.println("Invalid Command");
        return;
    }

    updateRelays();
    beep();
    sendStatus();
  }
}
