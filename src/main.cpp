#include <Arduino.h>

int ledPin = LED_BUILTIN;

void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop()
{
}
