#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define Log(...) Serial.println(__VA_ARGS__)

int ledPin = LED_BUILTIN;
int baudRate = 115200;
const char *ssid = "Gfiber208";
const char *password = "208walford";

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
