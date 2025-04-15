#include <Wire.h>
#include <Adafruit_BME280.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include "time.h"
#include <LittleFS.h>
#include <WebServer.h>

// ---------- WiFi ----------
const char *ssid = "NETIASPOT-Fqw9";
const char *password = "5f6bekxSmdekF";

// ---------- Web ----------
WebServer server(80);

const unsigned char thermometerIcon[] PROGMEM = {
    0xff, 0xf0, 0xff, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xc0, 0x3f, 0xff,
    0xff, 0xc0, 0x38, 0x7f, 0xff, 0xc0, 0x30, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xc0, 0x3f, 0xff,
    0xff, 0xc4, 0x38, 0x7f, 0xff, 0xc6, 0x38, 0x7f, 0xff, 0xc6, 0x3f, 0xff, 0xff, 0xc6, 0x3f, 0xff,
    0xff, 0xc6, 0x30, 0x7f, 0xff, 0xc6, 0x38, 0x7f, 0xff, 0xc6, 0x3f, 0xff, 0xff, 0xc6, 0x3f, 0xff,
    0xff, 0xc6, 0x30, 0x7f, 0xff, 0xc6, 0x3f, 0xff, 0xff, 0xc6, 0x3f, 0xff, 0xff, 0xc6, 0x3f, 0xff,
    0xff, 0x86, 0x1f, 0xff, 0xff, 0x0e, 0x0f, 0xff, 0xfe, 0x1f, 0x8f, 0xff, 0xfe, 0x39, 0x87, 0xff,
    0xfe, 0x30, 0xc7, 0xff, 0xfe, 0x30, 0xc7, 0xff, 0xfe, 0x39, 0x87, 0xff, 0xfe, 0x1f, 0x8f, 0xff,
    0xff, 0x0e, 0x0f, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xe0, 0xff, 0xff};
const unsigned char humidityIcon[] PROGMEM = {
    0xff, 0xfc, 0xff, 0xff, 0xef, 0xf8, 0x7f, 0xff, 0xe7, 0xfb, 0x7f, 0xff, 0xdb, 0xf7, 0xbf, 0xbf,
    0x9b, 0xf7, 0xbf, 0x5f, 0xbd, 0xef, 0xdf, 0x6f, 0x7d, 0xef, 0xde, 0xef, 0x7d, 0xdf, 0xec, 0xff,
    0xbd, 0xbf, 0xf5, 0xff, 0x9b, 0xbf, 0xf3, 0xfb, 0xc7, 0x7f, 0xfb, 0xf9, 0xff, 0x7f, 0xf7, 0xfd,
    0xfe, 0xff, 0xf7, 0xfd, 0xfc, 0xe7, 0xf7, 0xfe, 0xfd, 0xdb, 0xb7, 0xfe, 0xfb, 0xdb, 0x77, 0xfd,
    0xfb, 0xdb, 0x77, 0xfd, 0xf7, 0xda, 0xfb, 0xf9, 0xf7, 0xe6, 0xfd, 0xf3, 0xf7, 0xfc, 0xfe, 0x0f,
    0xf7, 0xfd, 0xff, 0xbf, 0xf7, 0xfd, 0x0f, 0xbf, 0xf7, 0xfb, 0x6f, 0xbf, 0xf7, 0xfb, 0x6f, 0xbf,
    0xf7, 0xf7, 0x6f, 0x7f, 0xfb, 0xff, 0x9f, 0x7f, 0xf9, 0xff, 0xfe, 0x7f, 0xfd, 0xff, 0xfe, 0xff,
    0xff, 0xff, 0xfd, 0xff, 0xff, 0x3f, 0xf3, 0xff, 0xff, 0x8f, 0xcf, 0xff, 0xff, 0xf0, 0x3f, 0xff};
const unsigned char barometerIcon[] PROGMEM = {
    0xff, 0xf0, 0x0f, 0xff, 0xff, 0x80, 0x01, 0xff, 0xfe, 0x0f, 0xf0, 0x7f, 0xfc, 0x3f, 0xfe, 0x3f,
    0xf8, 0xff, 0xff, 0x1f, 0xf1, 0xff, 0xff, 0x8f, 0xe3, 0xf8, 0x1f, 0xc7, 0xc7, 0xe0, 0x04, 0xe3,
    0xcf, 0x86, 0x60, 0xf3, 0x8f, 0x1e, 0x70, 0xf1, 0x9f, 0x3f, 0xf0, 0xf9, 0x9e, 0x7f, 0xe6, 0x79,
    0x3e, 0x7f, 0xce, 0x7c, 0x3c, 0xff, 0x9f, 0x3c, 0x3c, 0xff, 0x1f, 0x3c, 0x3c, 0x3f, 0x3c, 0x3c,
    0x3c, 0x3f, 0xfc, 0x3c, 0x3c, 0xff, 0xff, 0x3c, 0x3c, 0xff, 0xff, 0x3c, 0x3e, 0x7f, 0xfe, 0x7c,
    0x9e, 0x7f, 0xfe, 0x79, 0x9f, 0x3f, 0xfc, 0xf9, 0x8f, 0xfc, 0x3f, 0xe1, 0xcf, 0xfc, 0x3f, 0xc3,
    0xc7, 0xff, 0xff, 0xc3, 0xe3, 0xff, 0xff, 0x99, 0xf1, 0xf8, 0x1f, 0x98, 0xf8, 0xf8, 0x1f, 0x3c,
    0xfc, 0x3f, 0xfe, 0x3c, 0xfe, 0x0f, 0xf0, 0x18, 0xff, 0x80, 0x01, 0x81, 0xff, 0xf0, 0x0f, 0xc3};
// -- -- -- -- --Czujnik-- -- -- -- --Adafruit_BME280 bme;
Adafruit_BME280 bme;
// ---------- TFT ----------
TFT_eSPI tft = TFT_eSPI();
#define BTN_LEFT 38
#define BTN_RIGHT 39

// ---------- Zmienne ----------
int currentScreen = 0;
const int backgroundColor = TFT_WHITE;
const int textColor = TFT_BLACK;

float temperature, humidity, pressure;
float minTemp = 1000, maxTemp = -1000;
float minHumidity = 100, maxHumidity = 0;
float minPressure = 2000, maxPressure = 0;
unsigned long lastSensorRead = 0;
time_t lastResetTime = 0;

// ---------- Funkcje ----------
void syncTime()
{
  configTime(0, 0, "pool.ntp.org");
  while (time(nullptr) < 100000)
    delay(500);
  lastResetTime = time(nullptr);
}

void updateMinMax()
{
  time_t now = time(nullptr);
  if (difftime(now, lastResetTime) >= 86400)
  {
    minTemp = maxTemp = temperature;
    minHumidity = maxHumidity = humidity;
    minPressure = maxPressure = pressure;
    lastResetTime = now;
  }

  if (temperature < minTemp)
    minTemp = temperature;
  if (temperature > maxTemp)
    maxTemp = temperature;

  if (humidity < minHumidity)
    minHumidity = humidity;
  if (humidity > maxHumidity)
    maxHumidity = humidity;

  if (pressure < minPressure)
    minPressure = pressure;
  if (pressure > maxPressure)
    maxPressure = pressure;
}

String getFormattedTime()
{
  time_t now = time(nullptr);            // <- teraz to jest zmienna
  struct tm *timeinfo = localtime(&now); // <- tu przekazujemy jej adres

  char buffer[9];
  sprintf(buffer, "%02d:%02d", timeinfo->tm_hour + 2, timeinfo->tm_min); // +2 godziny (czas letni)
  return String(buffer);
}

void drawClock()
{
  tft.setTextDatum(TR_DATUM);
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, backgroundColor);
  tft.drawString(getFormattedTime(), 235, 5, 2);
}

void readSensor()
{
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0;
}

void showData()
{
  tft.fillScreen(backgroundColor);
  tft.setTextColor(textColor, backgroundColor);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  drawClock();

  switch (currentScreen)
  {
  case 0:
    tft.drawCentreString("Temperatura", 120, 20, 2);
    tft.setTextSize(1.5);
    tft.drawCentreString("Aktualna", 120, 50, 2);
    tft.drawCentreString(String(temperature, 1) + " C", 120, 70, 4);
    tft.setTextSize(1);
    tft.drawCentreString("Min:", 120, 120, 2);
    tft.drawCentreString(String(minTemp, 1) + " C", 120, 140, 2);
    tft.drawCentreString("Max:", 120, 165, 2);
    tft.drawCentreString(String(maxTemp, 1) + " C", 120, 185, 2);
    tft.drawBitmap(100, 230, thermometerIcon, 32, 32, TFT_DARKGREY);
    break;

  case 1:
    tft.drawCentreString("Wilgotnosc", 120, 20, 2);
    tft.setTextSize(1.5);
    tft.drawCentreString("Aktualna", 120, 50, 2);
    tft.drawCentreString(String(humidity, 1) + " %", 120, 70, 4);
    tft.setTextSize(1);
    tft.drawCentreString("Min:", 120, 120, 2);
    tft.drawCentreString(String(minHumidity, 1) + " %", 120, 140, 2);
    tft.drawCentreString("Max:", 120, 165, 2);
    tft.drawCentreString(String(maxHumidity, 1) + " %", 120, 185, 2);
    tft.drawBitmap(100, 230, humidityIcon, 32, 32, TFT_DARKGREY);
    break;

  case 2:
    tft.drawCentreString("Cisnienie", 120, 20, 2);
    tft.setTextSize(1.5);
    tft.drawCentreString("Aktualne", 120, 50, 2);
    tft.drawCentreString(String(pressure, 1) + " hPa", 120, 70, 4);
    tft.setTextSize(1);
    tft.drawCentreString("Min:", 120, 120, 2);
    tft.drawCentreString(String(minPressure, 1) + " hPa", 120, 140, 2);
    tft.drawCentreString("Max:", 120, 165, 2);
    tft.drawCentreString(String(maxPressure, 1) + " hPa", 120, 185, 2);
    tft.drawBitmap(100, 230, barometerIcon, 32, 32, TFT_DARKGREY);
    break;
  }
}

void handleSensorData()
{
  String json = "{";
  json += "\"temperature\":" + String(temperature) + ",";
  json += "\"humidity\":" + String(humidity) + ",";
  json += "\"pressure\":" + String(pressure) + ",";
  json += "\"minTemp\":" + String(minTemp) + ",";
  json += "\"maxTemp\":" + String(maxTemp) + ",";
  json += "\"minHumidity\":" + String(minHumidity) + ",";
  json += "\"maxHumidity\":" + String(maxHumidity) + ",";
  json += "\"minPressure\":" + String(minPressure) + ",";
  json += "\"maxPressure\":" + String(maxPressure) + ",";
  json += "\"time\":\"" + getFormattedTime() + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

// ---------- setup ----------
void setup()
{
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawCentreString("Laczenie z WiFi...", 120, 100, 2);

  WiFi.begin(ssid, password);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPołączono! IP: " + WiFi.localIP().toString());

  tft.fillScreen(TFT_BLACK);
  tft.drawCentreString("Synchronizacja czasu", 120, 100, 2);
  syncTime();

  if (!bme.begin(0x76) && !bme.begin(0x77))
  {
    tft.fillScreen(TFT_BLACK);
    tft.drawCentreString("BME280 not found!", 120, 100, 2);
    while (1)
      ;
  }

  tft.fillScreen(TFT_BLACK);
  tft.drawCentreString("BME280 OK!", 120, 100, 2);
  delay(1000);
  tft.fillScreen(TFT_BLACK);

  // Web
  if (!LittleFS.begin())
  {
    Serial.println("❌ Błąd LittleFS");
    return;
  }

  server.serveStatic("/", LittleFS, "/index.html");
  server.serveStatic("/style.css", LittleFS, "/style.css");
  server.serveStatic("/script.js", LittleFS, "/script.js");
  server.on("/sensor", handleSensorData);
  server.begin();
  Serial.println("🌐 Serwer uruchomiony!");
}

// ---------- loop ----------
void loop()
{
  if (digitalRead(BTN_LEFT) == LOW)
  {
    currentScreen = (currentScreen + 2) % 3;
    showData();
    delay(200);
  }

  if (digitalRead(BTN_RIGHT) == LOW)
  {
    currentScreen = (currentScreen + 1) % 3;
    showData();
    delay(200);
  }

  if (millis() - lastSensorRead > 10000)
  {
    readSensor();
    updateMinMax();
    showData();
    lastSensorRead = millis();
  }

  server.handleClient();
}