#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Bounce2.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "you_wifi_ssid";
const char* password = "you_wifi_password";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset = 7200; // GMT+2 (Kyiv time)
const int daylightOffset = 3600; // Daylight Saving Time (DST) offset

const int relayPin1 = D1;
const int relayPin2 = D2;

const int buttonPin1 = D5;
const int buttonPin2 = D6;

const char* apiKey = "openweathermap_api_key";
const char* city = "Ternopil";
const char* country = "UA";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset, daylightOffset);

Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();

bool manualControl1 = false;
bool manualControl2 = false;

const int manualOnHour1 = 10;
const int manualOffHour1 = 16;

const int manualOnHour2 = 18;
const int manualOffHour2 = 22;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  debouncer1.attach(buttonPin1);
  debouncer1.interval(50);

  debouncer2.attach(buttonPin2);
  debouncer2.interval(50);

  connectToWiFi();
  timeClient.begin();
}

void loop() {
  timeClient.update();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  int sunriseHour = getSunriseHour();
  int sunsetHour = getSunsetHour();
  int sunDuration = sunsetHour - sunriseHour;

  int timeShift = isSummer() ? 30 : 0;
  int shiftedSunriseHour = sunriseHour + timeShift;
  int shiftedSunsetHour = sunsetHour + timeShift;

  debouncer1.update();
  debouncer2.update();

  if (debouncer1.fell()) {
    toggleRelay(relayPin1);
  }

  if (debouncer2.fell()) {
    toggleRelay(relayPin2);
  }

  if (manualControl1) {
    digitalWrite(relayPin1, HIGH);
  } else if (currentHour >= shiftedSunriseHour && currentHour < shiftedSunsetHour) {
    digitalWrite(relayPin1, HIGH);
  } else if (currentHour >= manualOnHour1 && currentHour < manualOffHour1) {
    digitalWrite(relayPin1, HIGH);
  } else {
    digitalWrite(relayPin1, LOW);
  }

  if (manualControl2) {
    digitalWrite(relayPin2, HIGH);
  } else if (currentHour >= shiftedSunsetHour && currentHour < shiftedSunriseHour + 24) {
    digitalWrite(relayPin2, HIGH);
  } else if (currentHour >= manualOnHour2 && currentHour < manualOffHour2) {
    digitalWrite(relayPin2, HIGH);
  } else {
    digitalWrite(relayPin2, LOW);
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void toggleRelay(int pin) {
  if (pin == relayPin1) {
    manualControl1 = !manualControl1;
  } else if (pin == relayPin2) {
    manualControl2 = !manualControl2;
  }
  digitalWrite(pin, !digitalRead(pin));
}

int getSunriseHour() {
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + String(city) + "," + String(country) + "&appid=" + String(apiKey);
  HTTPClient http;

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 50;
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, payload);

    long sunrise = doc["sys"]["sunrise"];
    return hour(sunrise);
  } else {
    return 6;
  }

  http.end();
}

int getSunsetHour() {
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + String(city) + "," + String(country) + "&appid=" + String(apiKey);
  HTTPClient http;

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 50;
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, payload);

    long sunset = doc["sys"]["sunset"];
    return hour(sunset);
  } else {
    return 18;
  }

  http.end();
}

bool isSummer() {
  int currentMonth = month(timeClient.getEpochTime());
  return currentMonth >= 4 && currentMonth <= 9;
}