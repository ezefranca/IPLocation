# IPLocation [![Build and Run C++ IPLocation](https://github.com/ezefranca/IPLocation/actions/workflows/build.yml/badge.svg)](https://github.com/ezefranca/IPLocation/actions/workflows/build.yml)

A simple Arduino library to fetch geolocation information from your IP address using the [ip-api.com](http://ip-api.com) service.

## Features

- Auto-detect IP and get location data
- Optional support for querying custom IP or domain
- Language and field selection
- Parses and exposes result in a structured C++ object
- Fully compatible with ESP32 and ESP8266

## Supported Boards

- ESP32
- ESP8266
- Arduino UNO R4 WiFi (if using a compatible HTTP/JSON stack)

## Example Usage

```cpp
#include <WiFi.h>
#include <IPLocation.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

IPLocation ipLocation;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  ipLocation.setFields("status,message,country,city,lat,lon,timezone,query");
  ipLocation.setLanguage("en");

  if (ipLocation.fetch()) {
    IPLocationResult result = ipLocation.getResult();
    Serial.println(result.country);
    Serial.println(result.city);
  } else {
    Serial.println("Error: " + ipLocation.getLastError());
  }
}
```

## API Reference
```cpp
setQuery(String ipOrDomain)
setFields(String fieldsListOrCode)
setLanguage(String langCode)
fetch() // performs the HTTP query
getResult() // returns a IPLocationResult object
getLastError() // returns a string if something goes wrong
```


> ⚠️ The free version of ip-api.com is limited to 45 requests per minute. Respect their terms.

## License

[MIT](/LICENSE)
