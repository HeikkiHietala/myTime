#include "myTime.h"

myTime::myTime() {
    _timezone = "UTC";
    _ntpServer = "pool.ntp.org";
    _updateInterval = 3600000; // 1 hour
    _lastUpdate = 0;
    _offsetSeconds = 0;
    _ntpInitialized = false;
}

void myTime::initializeNTP() {
    if (!_ntpInitialized) {
        Serial.println("Initializing NTP...");
        // Simple UTC configuration - much faster than configTzTime
        configTime(0, 0, _ntpServer.c_str());
        _ntpInitialized = true;
    }
}

void myTime::setTimeZone(const String& tz) {
    _timezone = tz;
    // Recalculate offset immediately if we have valid time
    time_t now = time(nullptr);
    if (now > 946684800) {  // After year 2000
        _offsetSeconds = calculateTimezoneOffset(_timezone, now);
    }
}

void myTime::setNtpServer(const String& server) {
    _ntpServer = server;
    _ntpInitialized = false;  // Force reinitialization with new server
}

void myTime::setUpdateInterval(unsigned long intervalMs) {
    _updateInterval = intervalMs;
}

bool myTime::updateFromNTP() {
    initializeNTP();
    
    Serial.print("Syncing time");
    
    // Wait for valid time (not 1970)
    int retries = 20;
    time_t now = time(nullptr);
    
    while (now < 946684800 && retries > 0) {  // Before year 2000
        Serial.print(".");
        delay(500);
        now = time(nullptr);
        retries--;
    }
    
    if (now < 946684800) {
        Serial.println(" Failed!");
        return false;
    }
    
    Serial.println(" Success!");
    _lastUpdate = millis();
    
    // Calculate timezone offset
    _offsetSeconds = calculateTimezoneOffset(_timezone, now);
    
    return true;
}

void myTime::loopUpdate() {
    if (millis() - _lastUpdate >= _updateInterval) {
        updateFromNTP();
    }
}

int myTime::calculateTimezoneOffset(const String& tz, time_t utcTime) {
    // Handle custom offsets like "Asia/Kolkata+5:30"
    int plusIndex = tz.indexOf('+');
    int minusIndex = tz.indexOf('-');
    
    if (plusIndex > 0 || minusIndex > 0) {
        int index = (plusIndex > 0) ? plusIndex : minusIndex;
        String offsetStr = tz.substring(index);
        
        int colon = offsetStr.indexOf(':');
        if (colon >= 0) {
            int hours = offsetStr.substring(0, colon).toInt();
            int minutes = offsetStr.substring(colon + 1).toInt();
            return hours * 3600 + (hours >= 0 ? minutes : -minutes) * 60;
        } else {
            return offsetStr.toInt() * 3600;
        }
    }
    
    // Standard timezone offsets (simplified - doesn't handle all DST rules)
    // For full DST support, you'd need a timezone database
    
    String tzName = tz;
    tzName.toLowerCase();
    
    // Basic offsets for common timezones
    if (tzName == "utc") return 0;
    if (tzName == "europe/helsinki") return 7200 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "europe/paris") return 3600 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "europe/london") return 0 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "europe/berlin") return 3600 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "europe/moscow") return 10800;  // No DST since 2014
    if (tzName == "america/new_york") return -18000 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "america/los_angeles") return -28800 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "asia/tokyo") return 32400;
    if (tzName == "asia/shanghai") return 28800;
    if (tzName == "asia/kolkata") return 19800;
    if (tzName == "australia/sydney") return 36000 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "pacific/auckland") return 43200 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "america/sao_paulo") return -10800 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "africa/johannesburg") return 7200;
    
    return 0;  // Default to UTC if unknown
}

bool myTime::isDST(const String& tz, time_t utcTime) {
    // Simplified DST detection for common zones
    // Real implementation would need full timezone database
    
    struct tm* timeinfo = gmtime(&utcTime);
    int month = timeinfo->tm_mon + 1;
    int day = timeinfo->tm_mday;
    
    String tzName = tz;
    tzName.toLowerCase();
    
    // Europe (last Sunday March to last Sunday October)
    if (tzName.startsWith("europe/")) {
        if (month > 3 && month < 10) return true;
        if (month < 3 || month > 10) return false;
        // Simplified: just check month boundaries
        if (month == 3) return day >= 25;
        if (month == 10) return day < 25;
    }
    
    // US (second Sunday March to first Sunday November)
    if (tzName.startsWith("america/") && 
        (tzName.indexOf("new_york") >= 0 || tzName.indexOf("los_angeles") >= 0)) {
        if (month > 3 && month < 11) return true;
        if (month < 3 || month > 11) return false;
        if (month == 3) return day >= 8;
        if (month == 11) return day < 7;
    }
    
    // Australia (first Sunday October to first Sunday April)
    if (tzName == "australia/sydney") {
        if (month > 10 || month < 4) return true;
        if (month > 4 && month < 10) return false;
        if (month == 10) return day >= 1;
        if (month == 4) return day < 7;
    }
    
    return false;
}

int myTime::Y() {
    time_t now = time(nullptr) + _offsetSeconds;
    struct tm* t = gmtime(&now);
    return t->tm_year + 1900;
}

int myTime::Mon() {
    time_t now = time(nullptr) + _offsetSeconds;
    struct tm* t = gmtime(&now);
    return t->tm_mon + 1;
}

int myTime::D() {
    time_t now = time(nullptr) + _offsetSeconds;
    struct tm* t = gmtime(&now);
    return t->tm_mday;
}

int myTime::H() {
    time_t now = time(nullptr) + _offsetSeconds;
    struct tm* t = gmtime(&now);
    return t->tm_hour;
}

int myTime::M() {
    time_t now = time(nullptr) + _offsetSeconds;
    struct tm* t = gmtime(&now);
    return t->tm_min;
}

int myTime::S() {
    time_t now = time(nullptr) + _offsetSeconds;
    struct tm* t = gmtime(&now);
    return t->tm_sec;
}

time_t myTime::timestamp() {
    return time(nullptr) + _offsetSeconds;
}

String myTime::formatted() {
    char buf[9];
    sprintf(buf, "%02d:%02d:%02d", H(), M(), S());
    return String(buf);
}

String myTime::formattedDateTime() {
    char buf[20];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", Y(), Mon(), D(), H(), M(), S());
    return String(buf);
}
