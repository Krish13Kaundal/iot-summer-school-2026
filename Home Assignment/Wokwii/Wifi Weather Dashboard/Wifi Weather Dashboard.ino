#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------- WiFi Credentials --------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// -------- Pin Definitions --------
#define DHTPIN 4
#define DHTTYPE DHT22
#define LDR_PIN 34

// -------- Objects --------
WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {

  Serial.begin(115200);

  dht.begin();

  if (!bmp.begin()) {
    Serial.println("BMP180 not found!");
    while (1);
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (1);
  }

  display.clearDisplay();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();

}

void loop() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  float pressure = bmp.readPressure() / 100.0;     // hPa
  float altitude = bmp.readAltitude();

  int light = analogRead(LDR_PIN);

  // ---------- OLED ----------
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.print("Temp: ");
  display.print(temp);
  display.println(" C");

  display.print("Hum : ");
  display.print(hum);
  display.println(" %");

  display.print("Pres: ");
  display.print(pressure);
  display.println(" hPa");

  display.print("Alt : ");
  display.print(altitude);
  display.println(" m");

  display.print("Light:");
  display.println(light);

  display.display();

  // ---------- Serial ----------
  Serial.println("---------------------------");
  Serial.print("Temperature : ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Humidity    : ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Pressure    : ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Altitude    : ");
  Serial.print(altitude);
  Serial.println(" m");

  Serial.print("Light Value : ");
  Serial.println(light);

  // ---------- Web Server ----------
  WiFiClient client = server.available();

  if (client) {

    while (client.connected() && !client.available()) {
      delay(1);
    }

    while (client.available()) {
      client.read();
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();

    client.println("<!DOCTYPE html>");
    client.println("<html>");
    client.println("<head>");
    client.println("<meta http-equiv='refresh' content='10'>");
    client.println("<title>ESP32 Weather Dashboard</title>");
    client.println("</head>");
    client.println("<body style='font-family:Arial;'>");

    client.println("<h2>ESP32 Weather Dashboard</h2>");

    client.print("<h3>Temperature : ");
    client.print(temp);
    client.println(" &deg;C</h3>");

    client.print("<h3>Humidity : ");
    client.print(hum);
    client.println(" %</h3>");

    client.print("<h3>Pressure : ");
    client.print(pressure);
    client.println(" hPa</h3>");

    client.print("<h3>Altitude : ");
    client.print(altitude);
    client.println(" m</h3>");

    client.print("<h3>Light : ");
    client.print(light);
    client.println("</h3>");

    client.println("</body>");
    client.println("</html>");

    client.stop();
  }

  delay(5000);
}
