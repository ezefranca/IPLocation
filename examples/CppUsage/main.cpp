#include "src/IPLocation.h"
#include <iostream>

int main(int argc, char* argv[]) {
  IPLocation ip;

  if (argc > 1) {
#ifdef ARDUINO
    ip.setQuery(String(argv[1]));
#else
    ip.setQuery(std::string(argv[1]));
#endif
  } else {
    std::cout << "[INFO] No IP provided, using default (your own IP)\n";
    ip.setQuery("");  // Use public IP of host
  }

  if (ip.fetch()) {
    IPLocationResult r = ip.getResult();
    std::cout << "Country: " << r.country << "\n";
    std::cout << "City: " << r.city << "\n";
    std::cout << "Lat/Lon: " << r.lat << ", " << r.lon << "\n";
  } else {
    std::cout << "Error: " << ip.getLastError() << "\n";
    return 1;
  }

  return 0;
}