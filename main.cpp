#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <time.h>

#define DHTPIN 4
#define DHTTYPE DHT22

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* telegramUsername = "@YOUR_TELEGRAM_USERNAME";
const char* apiKey = "YOUR_CALLMEBOT_API_KEY";

float minTemp = 20.0;
float maxTemp = 26.0;

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);
String dataLog = "";

void reconnectWifi() {
  // retry wifi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost! Reconnecting...");
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reconnected!");
    } else {
      Serial.println("Reconnection failed.");
    }
  }
}

void sendAlert(String message) {
  HTTPClient http;
  String url = "https://api.callmebot.com/text.php?user=" + String(telegramUsername) + "&apikey=" + String(apiKey) + "&text=" + message;
  http.begin(url);
  http.GET();
  http.end();
}

String getTime() {
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);
  char buf[30];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
  return String(buf);
}

void handleRoot() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  String status = (temp > maxTemp || temp < minTemp) ? "🚨 ALERT" : "✅ SAFE";

  String html = "<html><head><title>Cold Chain Monitor</title>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<style>body{font-family:Arial;background:#0a0a0a;color:white;padding:20px}";
  html += "h1{color:#21ff7b}.card{background:#1a1a1a;border-radius:10px;padding:20px;margin:10px 0}";
  html += ".alert{color:#ff4444;font-weight:bold}.safe{color:#21ff7b}";
  html += "table{width:100%;border-collapse:collapse}";
  html += "th,td{padding:10px;border:1px solid #333;text-align:left}";
  html += "th{background:#21ff7b22;color:#21ff7b}</style></head><body>";
  html += "<h1>🌡️ Cold Chain Monitor</h1>";
  html += "<div class='card'><h2>Live Reading</h2>";
  html += "<p>Temperature: <b>" + String(temp) + " °C</b></p>";
  html += "<p>Humidity: <b>" + String(hum) + " %</b></p>";
  html += "<p>Time: " + getTime() + "</p>";
  html += "<p class='" + String(temp > maxTemp || temp < minTemp ? "alert" : "safe") + "'>" + status + "</p></div>";
  html += "<div class='card'><h2>Log</h2><table>";
  html += "<tr><th>Time</th><th>Temp</th><th>Humidity</th><th>Status</th></tr>";
  html += dataLog + "</table></div></body></html>";

  server.send(200, "text/html", html);
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
  Serial.println("\nConnected! Open: http://" + WiFi.localIP().toString());
  configTime(19800, 0, "pool.ntp.org");
  delay(2000);

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  reconnectWifi();
  server.handleClient();

  static unsigned long lastRead = 0;
  if (millis() - lastRead >= 10000) {
    lastRead = millis();

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    String status = "";

    if (temp > maxTemp) {
      status = "TOO HIGH";
      sendAlert("ALERT! Temp too HIGH: " + String(temp) + "C at " + getTime());
    } else if (temp < minTemp) {
      status = "TOO LOW";
      sendAlert("ALERT! Temp too LOW: " + String(temp) + "C at " + getTime());
    } else {
      status = "SAFE";
    }

    dataLog += "<tr><td>" + getTime() + "</td><td>" + String(temp) + "C</td><td>" + String(hum) + "%</td><td>" + status + "</td></tr>";
    Serial.println(getTime() + " | Temp: " + String(temp) + " | Humidity: " + String(hum) + " | " + status);
  }
}
