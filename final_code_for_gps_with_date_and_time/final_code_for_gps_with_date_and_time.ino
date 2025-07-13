#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial gpsSerial(3, 4); // RX = D3 (to TX of GPS), TX = D4 (not used)
TinyGPSPlus gps;

const int timezoneHourOffset = 5;
const int timezoneMinuteOffset = 30;

unsigned long lastPrintTime = 0;
const unsigned long updateInterval = 5000; // 5 seconds

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(115200);
  Serial.println("Initializing GPS...");
}

void loop() {
  // Keep reading and parsing GPS data
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // Check if 5 seconds have passed
  if (millis() - lastPrintTime >= updateInterval) {
    lastPrintTime = millis();

    Serial.println("----- GPS Update -----");

    if (gps.location.isValid()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
    } else {
      Serial.println("Waiting for valid location fix...");
    }

    if (gps.date.isValid()) {
      Serial.print("Date (DD/MM/YYYY): ");
      Serial.print(gps.date.day());
      Serial.print("/");
      Serial.print(gps.date.month());
      Serial.print("/");
      Serial.println(gps.date.year());
    } else {
      Serial.println("Date not valid yet.");
    }

    if (gps.time.isValid()) {
      int utcHour = gps.time.hour();
      int utcMinute = gps.time.minute();
      int utcSecond = gps.time.second();

      // Convert UTC to local time
      int localHour = utcHour + timezoneHourOffset;
      int localMinute = utcMinute + timezoneMinuteOffset;

      if (localMinute >= 60) {
        localMinute -= 60;
        localHour++;
      }
      if (localHour >= 24) {
        localHour -= 24;
      }

      Serial.print("Local Time (HH:MM:SS): ");
      if (localHour < 10) Serial.print("0");
      Serial.print(localHour); Serial.print(":");
      if (localMinute < 10) Serial.print("0");
      Serial.print(localMinute); Serial.print(":");
      if (utcSecond < 10) Serial.print("0");
      Serial.println(utcSecond);
    } else {
      Serial.println("Time not valid yet.");
    }

    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());
    Serial.println("-----------------------");
  }
}
