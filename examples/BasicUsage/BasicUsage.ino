#include <WiFi.h>               // or use <ESP8266WiFi.h> for ESP8266
#include <IPLocation.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

IPLocation ipLocation;

void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected.");

  // Optional configuration
  // ipLocation.setQuery("8.8.8.8"); // use specific IP or domain
  ipLocation.setFields("status,message,country,regionName,city,lat,lon,timezone,isp,query");
  ipLocation.setLanguage("en");

  Serial.println("Requesting IP location...");

  if (ipLocation.fetch()) {
    IPLocationResult result = ipLocation.getResult();

    Serial.println("GeoIP Lookup Success:");
    Serial.println("IP: " + result.query);
    Serial.println("Country: " + result.country);
    Serial.println("Region: " + result.regionName);
    Serial.println("City: " + result.city);
    Serial.println("Lat: " + String(result.lat, 6));
    Serial.println("Lon: " + String(result.lon, 6));
    Serial.println("Timezone: " + result.timezone);
    Serial.println("ISP: " + result.isp);
  } else {
    Serial.println("GeoIP Lookup Failed:");
    Serial.println(ipLocation.getLastError());
  }
}

void loop() {
  // nothing here
}