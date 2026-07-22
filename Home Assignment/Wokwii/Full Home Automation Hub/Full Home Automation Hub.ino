#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

#define MQ2_PIN 35
#define PIR_PIN 13
#define LDR_PIN 34

#define RELAY 26
#define BUZZER 14
#define RED_LED 25
#define GREEN_LED 2

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* topic = "iitjammu/yourname/home";

WiFiClient espClient;
PubSubClient client(espClient);
WebServer server(80);

float temperature = 0.0;
float humidity = 0.0;

int gasValue = 0;
int lightValue = 0;
bool motion = false;

bool relayState = false;
bool lightState = false;

unsigned long lastSensor = 0;
unsigned long lastMQTT = 0;

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Connection Failed!");
  }
}

void reconnectMQTT() {
  if (client.connected()) return;
  
  Serial.print("Attempting MQTT connection...");
  String clientId = "ESP32HomeHub-";
  clientId += String(random(0xffff), HEX);
  
  if (client.connect(clientId.c_str())) {
    Serial.println("connected");
  } else {
    Serial.print("failed, rc=");
    Serial.println(client.state());
  }
}

void readSensors() {
  float tempRead = dht.readTemperature();
  float humRead = dht.readHumidity();

  if (!isnan(tempRead)) temperature = tempRead;
  if (!isnan(humRead)) humidity = humRead;

  gasValue = analogRead(MQ2_PIN);
  lightValue = analogRead(LDR_PIN);
  motion = digitalRead(PIR_PIN);
}

void automationRules() {
  if (gasValue > 2500) {
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 2000);
    digitalWrite(RELAY, HIGH); 
    relayState = false;
  } 
  else {
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);

    if (temperature > 32.0) {
      relayState = true;
      digitalWrite(RELAY, LOW);
    } else if (temperature < 28.0) {
      relayState = false;
      digitalWrite(RELAY, HIGH);
    }
  }

  if (lightValue < 1500 && motion) {
    lightState = true;
    digitalWrite(GREEN_LED, HIGH);
  } else {
    lightState = false;
    digitalWrite(GREEN_LED, LOW);
  }
}

void updateOLED() {
  static bool page = false;
  page = !page;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (!page) {
    display.println("--- Environment ---");
    display.print("Temp : "); display.print(temperature, 1); display.println(" C");
    display.print("Hum  : "); display.print(humidity, 1); display.println(" %");
    display.print("Gas  : "); display.println(gasValue);
    display.print("LDR  : "); display.println(lightValue);
  } else {
    display.println("--- Automation ---");
    display.print("Motion: "); display.println(motion ? "YES" : "NO");
    display.print("Fan   : "); display.println(relayState ? "ON" : "OFF");
    display.print("Light : "); display.println(lightState ? "ON" : "OFF");
  }

  display.display();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>ESP32 Home Hub</title>";
  html += "</head><body>";
  html += "<h2>ESP32 Home Automation Hub</h2>";
  html += "<p><b>Temperature:</b> " + String(temperature, 1) + " C</p>";
  html += "<p><b>Humidity:</b> " + String(humidity, 1) + " %</p>";
  html += "<hr>";
  html += "<p><b>Gas Level:</b> " + String(gasValue) + "</p>";
  html += "<p><b>Light Level:</b> " + String(lightValue) + "</p>";
  html += "<p><b>Motion:</b> " + String(motion ? "Detected" : "Clear") + "</p>";
  html += "<p><b>Fan State:</b> " + String(relayState ? "ON" : "OFF") + "</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void publishMQTT() {
  if (!client.connected()) {
    reconnectMQTT();
  }

  if (client.connected()) {
    String payload = "{";
    payload += "\"temp\":" + String(temperature, 1) + ",";
    payload += "\"humidity\":" + String(humidity, 1) + ",";
    payload += "\"gas\":" + String(gasValue) + ",";
    payload += "\"light\":" + String(lightValue) + ",";
    payload += "\"motion\":" + String(motion ? 1 : 0) + ",";
    payload += "\"fan\":\"" + String(relayState ? "ON" : "OFF") + "\"";
    payload += "}";

    client.publish(topic, payload.c_str());
    Serial.println("MQTT Published: " + payload);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  digitalWrite(RELAY, HIGH);

  dht.begin();
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
  }

  connectWiFi();
  client.setServer(mqttServer, mqttPort);

  server.on("/", handleRoot);
  server.begin();

  Serial.print("Open Browser: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() - lastSensor > 5000) {
    lastSensor = millis();
    readSensors();
    automationRules();
    updateOLED();
  }

  if (millis() - lastMQTT > 30000) {
    lastMQTT = millis();
    publishMQTT();
  }

  server.handleClient();

  if (client.connected()) {
    client.loop();
  }
}
