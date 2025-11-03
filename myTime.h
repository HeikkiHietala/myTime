#ifndef MYTIME_H
#define MYTIME_H

#include <Arduino.h>
#include "time.h"

class myTime {
private:
    time_t _timestamp;              // current epoch time
    String _timezone;               // timezone string
    String _ntpServer;              // NTP server
    unsigned long _updateInterval;  // update interval in ms
    unsigned long _lastUpdate;      // last update time

    float _offsetHours;             // optional UTC offset in hours

    void parseTimeZone(const String& tz);  // internal parser for IANA+offset

public:
    myTime();

    // Configuration
    void setTimeZone(const String& tz);   // accepts IANA name or IANA+offset like "Asia/Kolkata+5:30"
    void setNtpServer(const String& server);
    void setUpdateInterval(unsigned long intervalMs);

    // Update
    void updateFromNTP();                 // force NTP sync
    void loopUpdate();                    // periodic NTP update

    // Accessors
    int Y();   // year
    int Mon(); // month 1-12
    int D();   // day
    int H();   // hour
    int M();   // minute
    int S();   // second

    time_t timestamp();
    String formatted();          // HH:MM:SS
    String formattedDateTime();  // YYYY-MM-DD HH:MM:SS
};

// Convenience IANA timezone list (partial, extend as desired)
static const char* IANATimeZones[] = {
    "UTC",
    "Europe/Helsinki",
    "Europe/Paris",
    "Europe/London",
    "Europe/Berlin",
    "Europe/Moscow",
    "America/New_York",
    "America/Los_Angeles",
    "Asia/Tokyo",
    "Asia/Shanghai",
    "Asia/Kolkata",
    "Australia/Sydney",
    "Pacific/Auckland",
    "America/Sao_Paulo",
    "Africa/Johannesburg"
};

static const size_t IANATimeZonesCount = sizeof(IANATimeZones)/sizeof(IANATimeZones[0]);

#endif
