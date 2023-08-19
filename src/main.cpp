#include <Arduino.h>
#include <ESP8266WiFi.h>

#define Log(...) Serial.println(__VA_ARGS__)

int ledPin = LED_BUILTIN;
int baudRate = 115200;
const char *ssid = "Gfiber208";
const char *password = "208walford";

WiFiServer server(80);

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
    delay(500);
    Serial.print(".");
  }
  Log("IP address:");
  Log(WiFi.localIP());
}

void loop()
{
}
