#ifndef MYTIME_H
#define MYTIME_H

#include <Arduino.h>
#include <time.h>
#include <sys/time.h>

class myTime {
private:
    String _timezone;               // timezone string (IANA name)
    String _ntpServer;              // NTP server
    unsigned long _updateInterval;  // update interval in ms
    unsigned long _lastUpdate;      // last update time
    int _offsetSeconds;             // UTC offset in seconds (includes DST)
    bool _ntpInitialized;           // track if NTP is initialized
    
    void initializeNTP();           // one-time NTP initialization
    int calculateTimezoneOffset(const String& tz, time_t utcTime);  // calculate offset for timezone
    bool isDST(const String& tz, time_t utcTime);  // check if DST is active
    
public:
    myTime();
    
    // Configuration
    void setTimeZone(const String& tz);
    void setNtpServer(const String& server);
    void setUpdateInterval(unsigned long intervalMs);
    
    // Update
    bool updateFromNTP();            // force NTP sync, returns true if successful
    void loopUpdate();               // periodic NTP update
    
    // Accessors (all use current system time + offset)
    int Y();   // year
    int Mon(); // month 1-12
    int D();   // day
    int H();   // hour
    int M();   // minute
    int S();   // second
    time_t timestamp();              // current unix timestamp with timezone applied
    String formatted();              // HH:MM:SS
    String formattedDateTime();      // YYYY-MM-DD HH:MM:SS
};

// Convenience IANA timezone list
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
