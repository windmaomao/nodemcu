#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Log(...) Serial.println(__VA_ARGS__)

int ledPin = LED_BUILTIN;
int baudRate = 115200;
const char *ssid = "Gfiber208";
const char *password = "208walford";

Adafruit_SSD1306 display(128, 32, &Wire, -1);
ESP8266WebServer server(80);

void flashLed(int duration = 500)
{
  digitalWrite(ledPin, HIGH);
  delay(duration);
  digitalWrite(ledPin, LOW);
  delay(duration);
}

void rootRoute()
{
  server.send(200, "text/plain", "Hello World!\r\n");
  flashLed();
}

void setup()
{
  // setup the pullup led
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // setup serial
  Serial.begin(baudRate);
  Log("Start logging...");

  // setup oled
  Serial.print("Oled allocating");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    flashLed();
    Serial.print(".");
  }
  Log("");
  display.display();
  flashLed();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Initializing...");
  display.display();

  // setup wifi
  WiFi.begin(ssid, password);
  Serial.print("Wifi connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    flashLed();
    Serial.print(".");
  }
  Log("IP address:");
  Log(WiFi.localIP());

  // Setup server
  server.on("/", rootRoute);
  Log("Route /");
  server.begin();
  Log("Http server started.");
}

void loop()
{
  server.handleClient();
}
