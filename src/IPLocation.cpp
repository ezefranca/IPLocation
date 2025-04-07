#include "IPLocation.h"

IPLocation::IPLocation() {}

void IPLocation::setQuery(const String& query) {
  _query = query;
}

void IPLocation::setLanguage(const String& lang) {
  _lang = lang;
}

void IPLocation::setFields(const String& fields) {
  _fields = fields;
}

bool IPLocation::fetch() {
  HTTPClient http;
  String url = "http://ip-api.com/json/";

  if (_query.length() > 0) {
    url += _query;
  }

  url += "?";
  if (_fields.length() > 0) {
    url += "fields=" + _fields + "&";
  }
  if (_lang.length() > 0) {
    url += "lang=" + _lang;
  }

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      http.end();
      return parseResponse(payload);
    } else {
      _lastError = "HTTP error: " + String(httpCode);
    }
  } else {
    _lastError = "Connection failed";
  }

  http.end();
  return false;
}

bool IPLocation::parseResponse(const String& payload) {
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    _lastError = "JSON parse error";
    return false;
  }

  String status = doc["status"] | "fail";
  if (status != "success") {
    _lastError = doc["message"] | "unknown error";
    return false;
  }

  _result.success = true;
  _result.message = "";
  _result.continent = doc["continent"] | "";
  _result.continentCode = doc["continentCode"] | "";
  _result.country = doc["country"] | "";
  _result.countryCode = doc["countryCode"] | "";
  _result.region = doc["region"] | "";
  _result.regionName = doc["regionName"] | "";
  _result.city = doc["city"] | "";
  _result.district = doc["district"] | "";
  _result.zip = doc["zip"] | "";
  _result.lat = doc["lat"] | 0.0;
  _result.lon = doc["lon"] | 0.0;
  _result.timezone = doc["timezone"] | "";
  _result.offset = doc["offset"] | 0;
  _result.currency = doc["currency"] | "";
  _result.isp = doc["isp"] | "";
  _result.org = doc["org"] | "";
  _result.asn = doc["as"] | "";
  _result.asname = doc["asname"] | "";
  _result.reverse = doc["reverse"] | "";
  _result.mobile = doc["mobile"] | false;
  _result.proxy = doc["proxy"] | false;
  _result.hosting = doc["hosting"] | false;
  _result.query = doc["query"] | "";

  return true;
}

IPLocationResult IPLocation::getResult() {
  return _result;
}

String IPLocation::getLastError() {
  return _lastError;
}