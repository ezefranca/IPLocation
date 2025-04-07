#include "IPLocation.h"

#ifdef ARDUINO
  #include <WiFiClient.h>
  #include <HTTPClient.h>
  #include <ArduinoJson.h>

IPLocation::IPLocation() {}

void IPLocation::setQuery(const String& query) { _query = query; }

bool IPLocation::fetch() {
  if (WiFi.status() != WL_CONNECTED) {
    _lastError = "WiFi not connected";
    return false;
  }

  HTTPClient http;
  String url = "http://ip-api.com/json/";
  if (_query.length()) url += _query;

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    http.end();
    return parseResponse(payload);
  } else {
    _lastError = "HTTP error: " + String(httpCode);
    http.end();
    return false;
  }
}

bool IPLocation::parseResponse(const String& payload) {
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    _lastError = "JSON parse error";
    return false;
  }

  if (doc["status"] != "success") {
    _lastError = doc["message"] | "Unknown error";
    return false;
  }

  _result.success = true;
  _result.country = doc["country"] | "";
  _result.city = doc["city"] | "";
  _result.lat = doc["lat"] | 0.0;
  _result.lon = doc["lon"] | 0.0;
  _result.query = doc["query"] | "";
  return true;
}

IPLocationResult IPLocation::getResult() {
  return _result;
}

String IPLocation::getLastError() { return _lastError; }

#else
  // ===== Desktop version using raw sockets + simple parsing =====
  #include <netdb.h>
  #include <unistd.h>
  #include <sstream>
  #include <cstring>
  #include <iostream>

  IPLocation::IPLocation() {}

  void IPLocation::setQuery(const std::string& query) { _query = query; }

  static std::string http_get_socket(const std::string& host, const std::string& path) {
    int sockfd;
    struct hostent* server;
    struct sockaddr_in serv_addr;
    const int port = 80;

    server = gethostbyname(host.c_str());
    if (!server) throw std::runtime_error("DNS resolution failed");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) throw std::runtime_error("Socket creation failed");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      close(sockfd);
      throw std::runtime_error("Connection failed");
    }

    std::ostringstream req;
    req << "GET " << path << " HTTP/1.1\r\n"
        << "Host: " << host << "\r\n"
        << "Connection: close\r\n"
        << "\r\n";

    std::string request = req.str();
    send(sockfd, request.c_str(), request.length(), 0);

    std::string response;
    char buffer[1024];
    ssize_t bytes;
    while ((bytes = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
      buffer[bytes] = '\0';
      response += buffer;
    }

    close(sockfd);

    size_t header_end = response.find("\r\n\r\n");
    if (header_end == std::string::npos) throw std::runtime_error("Invalid HTTP response");

    return response.substr(header_end + 4);
  }

  bool IPLocation::fetch() {
    try {
      std::string path = "/json";
      if (!_query.empty()) path += "/" + _query;
      std::string json = http_get_socket("ip-api.com", path);
      return parseResponse(json);
    } catch (const std::exception& e) {
      _lastError = std::string("Fetch failed: ") + e.what();
      return false;
    }
  }

  bool IPLocation::parseResponse(const std::string& payload) {
    // ⚠️ MINIMAL JSON parsing: not robust, but works without libraries
    auto get_val = [&](const std::string& key) -> std::string {
      size_t start = payload.find("\"" + key + "\":");
      if (start == std::string::npos) return "";
      start = payload.find(":", start) + 1;
      while (start < payload.size() && (payload[start] == ' ' || payload[start] == '\"')) start++;
      size_t end = payload.find_first_of(",}\"", start);
      return payload.substr(start, end - start);
    };

    std::string status = get_val("status");
    if (status != "success") {
      _lastError = get_val("message");
      return false;
    }

    _result.success = true;
    _result.country = get_val("country");
    _result.city = get_val("city");
    _result.query = get_val("query");

    try {
      _result.lat = std::stof(get_val("lat"));
      _result.lon = std::stof(get_val("lon"));
    } catch (...) {
      _result.lat = 0.0f;
      _result.lon = 0.0f;
    }

    return true;
  }

IPLocationResult IPLocation::getResult() {
  return _result;
}

  std::string IPLocation::getLastError() { return _lastError; }

#endif