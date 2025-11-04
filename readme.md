# myTime

Lightweight ESP32 Arduino library for NTP-synced, timezone-aware, DST-aware date and time.

## Features

- NTP synchronization with configurable server
- IANA timezone support
- Optional UTC offsets in timezone strings (e.g., `"Asia/Kolkata+5:30"`)
- Automatic DST handling
- Full date and time access: `Y()`, `Mon()`, `D()`, `H()`, `M()`, `S()`
- Formatted output: `formatted()` (`HH:MM:SS`) and `formattedDateTime()` (`YYYY-MM-DD HH:MM:SS`)
- Automatic periodic updates with configurable interval
- Built-in convenience list of IANA timezones

## Installation

1. Download the ZIP from GitHub or clone the repository.
2. In Arduino IDE: **Sketch → Include Library → Add .ZIP Library…** and select the ZIP.

## Usage Examples

### 1. Basic NTP Sync
```cpp
#include <WiFi.h>
#include <myTime.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

myTime clock;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    
    clock.updateFromNTP();   // fetch UTC time
    Serial.println(clock.formattedDateTime());
}

void loop() {}
```

### 2. Custom NTP Server
```cpp
clock.setNtpServer("time.google.com"); // use Google NTP server
clock.updateFromNTP();
```

### 3. Timezone and DST (using IANA names)
```cpp
// Set timezone using IANA name
clock.setTimeZone("Europe/Helsinki");
clock.updateFromNTP();
Serial.println(clock.formattedDateTime());
```

You can also use the built-in timezone list:
```cpp
clock.setTimeZone(IANATimeZones[1]);  // Europe/Helsinki
```

### 4. Timezone with UTC Offset
```cpp
clock.setTimeZone("Asia/Kolkata+5:30"); // UTC+5:30
clock.updateFromNTP();
Serial.println(clock.formattedDateTime());
```

Optional offset allows fixed time zones not in the IANA database or special adjustments.

DST is applied automatically for IANA names, ignored for fixed offsets.

### 5. Access Year, Month, Day, Hour, Minute, Second
```cpp
int year  = clock.Y();
int month = clock.Mon();
int day   = clock.D();
int hour  = clock.H();
int min   = clock.M();
int sec   = clock.S();

Serial.printf("Date: %04d-%02d-%02d\n", year, month, day);
Serial.printf("Time: %02d:%02d:%02d\n", hour, min, sec);
```

### 6. Formatted Strings
```cpp
Serial.println(clock.formatted());         // HH:MM:SS
Serial.println(clock.formattedDateTime()); // YYYY-MM-DD HH:MM:SS
```

### 7. Periodic Automatic Updates
```cpp
clock.setUpdateInterval(60000); // update every 60 seconds

void loop() {
    clock.loopUpdate();           // call in loop
    Serial.println(clock.formattedDateTime());
    delay(1000);
}
```

`setUpdateInterval(ms)` sets the period between NTP updates.

`loopUpdate()` checks the interval and fetches time from NTP when needed.

### 8. Full Example
```cpp
#include <WiFi.h>
#include <myTime.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

myTime clock;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    
    clock.setTimeZone("Europe/Helsinki");
    // clock.setTimeZone("Asia/Kolkata+5:30");
    clock.setNtpServer("time.google.com");
    clock.setUpdateInterval(60000);
    clock.updateFromNTP();
}

void loop() {
    clock.loopUpdate();
    Serial.println(clock.formattedDateTime());
    delay(1000);
}
```

### 9. Built-in Timezone List
```cpp
for (size_t i = 0; i < IANATimeZonesCount; i++) {
    Serial.println(IANATimeZones[i]);
}

// Example usage
clock.setTimeZone(IANATimeZones[1]); // Europe/Helsinki
```

## License

MIT License
