# myTime

ESP32 Arduino library for NTP-synced, timezone-aware, DST-aware date and time.

## Features

- NTP synchronization with configurable server
- IANA timezone support
- Optional UTC offsets in timezone strings (e.g., "Asia/Kolkata+5:30")
- Automatic DST handling
- Full date and time access: `Y()`, `Mon()`, `D()`, `H()`, `M()`, `S()`
- Formatted output: `formatted()` (`HH:MM:SS`) and `formattedDateTime()` (`YYYY-MM-DD HH:MM:SS`)
- Automatic periodic updates with configurable interval
- Built-in convenience list of IANA timezones

---
## Access Individual time parts

int year  = clock.Y();
int month = clock.Mon();
int day   = clock.D();
int hour  = clock.H();
int min   = clock.M();
int sec   = clock.S();


## Usage Example

#include <WiFi.h>
#include <myTime.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

myTime clock;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);

    // Set timezone
    clock.setTimeZone("Europe/Helsinki");       // IANA
    // or with offset
    // clock.setTimeZone("Asia/Kolkata+5:30");

    clock.setNtpServer("time.google.com");
    clock.setUpdateInterval(60000);            // 60 sec
    clock.updateFromNTP();

    Serial.println("Local time: " + clock.formattedDateTime());
}

void loop() {
    clock.loopUpdate();
    Serial.println(clock.formattedDateTime());
    delay(1000);
}
