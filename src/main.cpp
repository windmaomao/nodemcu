#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Log(...) Serial.println(__VA_ARGS__)

int ledPin = LED_BUILTIN;
int buzzPin = D5;
int baudRate = 115200;
const char *ssid = "Gfiber208";
const char *password = "208walford";

Adafruit_SSD1306 display(128, 32, &Wire, -1);
ESP8266WebServer server(80);

WiFiClient wlanclient;
PubSubClient mqttClient(wlanclient);

const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Sophia / Lucas</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h3>Hi Sophia and Lucas</h3>
  <form action="/">
    Type something: <input type="text" name="text" autofocus />
    <input type="submit" value="Submit" />
  </form><br>
</body></html>)rawliteral";

void flashLed(int duration = 500)
{
  digitalWrite(ledPin, LOW);
  delay(duration);
  digitalWrite(ledPin, HIGH);
  delay(duration);
}

void playTone(double freq)
{
  tone(buzzPin, freq, 500);
}

void displayText(const char *text, int line = 1)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 12 * line);
  display.println(text);
  display.display();
}

void rootRoute()
{
  display.clearDisplay();

  const char *message = "Hi Sopha and Lucas!";
  displayText(message);
  // server.send(200, "text/plain", message);

  String arg = server.arg("text");
  const char *text = arg.isEmpty() ? "..." : arg.c_str();
  displayText(text, 1);

  server.send(200, "text/html", html);
}

void mqttCallback(char *topic, uint8_t *payload, unsigned int length)
{
  String str = "";
  for (unsigned int i = 0; i < length; i++)
  {
    str += (char)payload[i];
  }
  Log(str);
  displayText(str.c_str());
  playTone(payload[0] << 1);
}

void setup()
{
  // setup the pullup led
  pinMode(ledPin, OUTPUT);
  flashLed();

  // setup the buzzer
  pinMode(buzzPin, OUTPUT);
  noTone(buzzPin);
  playTone(523);

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
  displayText("Initializing...");

  // setup wifi
  WiFi.begin(ssid, password);
  Serial.print("Wifi connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    flashLed();
    Serial.print(".");
  }
  Log("");
  Log("IP address:");
  Log(WiFi.localIP());
  displayText(WiFi.localIP().toString().c_str());

  // Setup http server
  server.on("/", rootRoute);
  server.begin();
  Log("Http server started.");

  // Setup mtqq broker
  mqttClient.setServer("broker.emqx.io", 1883);
  mqttClient.setCallback(mqttCallback);

  if (mqttClient.connect("Test", NULL, NULL))
  {
    Log("MQTT connected.");
    mqttClient.subscribe("test");
  }
  else
  {
    Log("MQTT failed.");
    Log(mqttClient.state());
  }
}

void loop()
{
  server.handleClient();
  mqttClient.loop();
}
