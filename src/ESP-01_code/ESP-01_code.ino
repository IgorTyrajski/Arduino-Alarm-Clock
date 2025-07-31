#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

const long utcOffsetInSeconds = 2 * 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

String serverPath;

String lastWeatherDescription = "Brak danych";
float lastTemperature = 0.0;
float lastFeelsLike = 0.0;
int lastHumidity = 0;


unsigned long previousWeatherUpdate = 0;
const unsigned long weatherInterval = 60000; // 60 sekund

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();

  // URL do API (zmienne z "secrets.h")
  serverPath = "http://api.openweathermap.org/data/2.5/weather?q=";
  serverPath += city;
  serverPath += ",";
  serverPath += country;
  serverPath += "&appid=";
  serverPath += apiKey;
  serverPath += "&units=metric";
  serverPath += "&lang=pl";  
}

void loop() {
  timeClient.update(); 

  unsigned long currentMillis = millis();

  
  if (currentMillis - previousWeatherUpdate >= weatherInterval) {
    previousWeatherUpdate = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      WiFiClient client;
      http.begin(client, serverPath);
      int httpCode = http.GET();

      if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        lastWeatherDescription = doc["weather"][0]["description"].as<String>();
        lastTemperature = doc["main"]["temp"];
        lastFeelsLike = doc["main"]["feels_like"];
        lastHumidity = doc["main"]["humidity"];
      } else {
        Serial.print("Błąd HTTP: ");
        Serial.println(httpCode);
      }

      http.end();
    } else {
      Serial.println("Brak polaczenia z WiFi.");
    }
  }
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", hours, minutes);  // np. "14:07"

  Serial.print("time:");
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(",");
  Serial.println(timeStr);

  Serial.print("description:");
  Serial.println(lastWeatherDescription);

  Serial.print("feels_like:");
  Serial.println(lastFeelsLike);

  Serial.print("humidity:");
  Serial.println(lastHumidity);

  delay(1000);
}
