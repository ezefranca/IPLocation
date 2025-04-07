#ifndef IPLOCATION_H
#define IPLOCATION_H

#ifdef ARDUINO
  #include <Arduino.h>
#else
  #include <string>
#endif

class IPLocationResult {
public:
  bool success = false;
#ifdef ARDUINO
  String message, country, city, query;
#else
  std::string message, country, city, query;
#endif
  float lat = 0.0;
  float lon = 0.0;
};

class IPLocation {
public:
  IPLocation();
#ifdef ARDUINO
  void setQuery(const String& query);
  bool fetch();
  String getLastError();
#else
  void setQuery(const std::string& query);
  bool fetch();
  std::string getLastError();
#endif
  IPLocationResult getResult();

private:
#ifdef ARDUINO
  String _query = "";
  String _lastError = "";
  bool parseResponse(const String& payload);
#else
  std::string _query = "";
  std::string _lastError = "";
  bool parseResponse(const std::string& payload);
#endif
  IPLocationResult _result;
};

#endif