#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define DHTPIN 4
#define DHTTYPE DHT22

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* telegramUsername = "@YOUR_TELEGRAM_USERNAME";
const char* apiKey = "YOUR_CALLMEBOT_API_KEY";

float minTemp = 20.0;
float maxTemp = 26.0;

DHT dht(DHTPIN, DHTTYPE);

void sendAlert(String message) {
  // sending telegram alert when temp is out of range
  HTTPClient http;
  String url = "https://api.callmebot.com/text.php?user=" + String(telegramUsername) + "&apikey=" + String(apiKey) + "&text=" + message;
  http.begin(url);
  http.GET();
  http.end();
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void loop() {
  delay(10000);

  // asking dht22 for readings
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("C | Humidity: ");
  Serial.print(hum);
  Serial.println("%");

  // check if temp is out of safe range
  if (temp > maxTemp) {
    Serial.println("ALERT: Temp too HIGH!");
    sendAlert("ALERT! Temp too HIGH: " + String(temp) + "C");
  } else if (temp < minTemp) {
    Serial.println("ALERT: Temp too LOW!");
    sendAlert("ALERT! Temp too LOW: " + String(temp) + "C");
  } else {
    Serial.println("Temp is safe.");
  }
}
