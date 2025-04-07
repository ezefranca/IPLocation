// src/IPLocation.h
#ifndef IPLOCATION_H
#define IPLOCATION_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class IPLocationResult {
public:
  bool success = false;
  String message = "";
  String continent = "";
  String continentCode = "";
  String country = "";
  String countryCode = "";
  String region = "";
  String regionName = "";
  String city = "";
  String district = "";
  String zip = "";
  float lat = 0.0;
  float lon = 0.0;
  String timezone = "";
  int offset = 0;
  String currency = "";
  String isp = "";
  String org = "";
  String asn = "";
  String asname = "";
  String reverse = "";
  bool mobile = false;
  bool proxy = false;
  bool hosting = false;
  String query = "";
};

class IPLocation {
public:
  IPLocation();
  void setQuery(const String& query); // optional IP or domain
  void setLanguage(const String& lang); // optional language code
  void setFields(const String& fields); // optional field list or numeric field code
  bool fetch();
  IPLocationResult getResult();
  String getLastError();

private:
  String _query = "";
  String _lang = "";
  String _fields = "";
  String _lastError = "";
  IPLocationResult _result;

  bool parseResponse(const String& payload);
};

#endif
