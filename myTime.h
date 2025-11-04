#ifndef MYTIME_H
#define MYTIME_H

#include <Arduino.h>
#include <time.h>
#include <sys/time.h>

// ============================================================================
// myTime class definition
// ============================================================================
class myTime {
private:
    String _timezone;
    String _ntpServer;
    unsigned long _updateInterval;
    unsigned long _lastUpdate;
    int _offsetSeconds;
    bool _ntpInitialized;
    bool _useFixedOffset;

    void initializeNTP();
    int calculateTimezoneOffset(const String& tz, time_t utcTime);
    bool isDST(const String& tz, time_t utcTime);

public:
    myTime();

    void setTimeZone(const String& tz);
    void setNtpServer(const String& server);
    void setUpdateInterval(unsigned long intervalMs);
    void setFixedOffset(int seconds);

    bool updateFromNTP();
    void loopUpdate();

    int Y();
    int Mon();
    int D();
    int H();
    int M();
    int S();
    time_t timestamp();
    String formatted();
    String formattedDateTime();
};

// ============================================================================
// Built-in IANA Timezone List (one representative per region)
// ============================================================================
static const char* IANATimeZones[] = {
    "UTC",
    "Europe/Helsinki",        // Finland
    "Europe/London",          // United Kingdom
    "Europe/Paris",           // France
    "Europe/Berlin",          // Germany
    "Europe/Madrid",          // Spain
    "Europe/Rome",            // Italy
    "Europe/Warsaw",          // Poland
    "Europe/Athens",          // Greece
    "Europe/Moscow",          // Russia
    "Asia/Tokyo",             // Japan
    "Asia/Seoul",             // South Korea
    "Asia/Shanghai",          // China
    "Asia/Hong_Kong",         // Hong Kong
    "Asia/Kolkata",           // India
    "Asia/Dubai",             // UAE
    "Asia/Bangkok",           // Thailand
    "Asia/Singapore",         // Singapore
    "Asia/Jakarta",           // Indonesia
    "Australia/Sydney",       // Australia
    "Pacific/Auckland",       // New Zealand
    "Africa/Cairo",           // Egypt
    "Africa/Johannesburg",    // South Africa
    "America/New_York",       // USA (Eastern)
    "America/Chicago",        // USA (Central)
    "America/Denver",         // USA (Mountain)
    "America/Los_Angeles",    // USA (Pacific)
    "America/Toronto",        // Canada (Eastern)
    "America/Vancouver",      // Canada (Pacific)
    "America/Sao_Paulo",      // Brazil
    "America/Buenos_Aires",   // Argentina
    "America/Mexico_City",    // Mexico
    "America/Lima",           // Peru
    "America/Bogota",         // Colombia
    "America/Santiago",       // Chile
    "America/Anchorage",      // Alaska
    "Pacific/Honolulu"        // Hawaii
};

// Friendly city display names (same order)
static const char* IANATimeZoneCities[] = {
    "UTC",
    "Helsinki",
    "London",
    "Paris",
    "Berlin",
    "Madrid",
    "Rome",
    "Warsaw",
    "Athens",
    "Moscow",
    "Tokyo",
    "Seoul",
    "Shanghai",
    "Hong Kong",
    "Kolkata",
    "Dubai",
    "Bangkok",
    "Singapore",
    "Jakarta",
    "Sydney",
    "Auckland",
    "Cairo",
    "Johannesburg",
    "New York",
    "Chicago",
    "Denver",
    "Los Angeles",
    "Toronto",
    "Vancouver",
    "São Paulo",
    "Buenos Aires",
    "Mexico City",
    "Lima",
    "Bogotá",
    "Santiago",
    "Anchorage",
    "Honolulu"
};

static const size_t IANATimeZonesCount = sizeof(IANATimeZones) / sizeof(IANATimeZones[0]);

#endif
