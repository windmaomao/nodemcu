#include <Arduino.h>

#define Log(...) Serial.println(__VA_ARGS__)

int ledPin = LED_BUILTIN;
int baudRate = 115200;

void setup()
{
  // setup the pullup led
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  // setup serial
  Serial.begin(baudRate);
  Log("Start logging...");
}

void loop()
{
}
