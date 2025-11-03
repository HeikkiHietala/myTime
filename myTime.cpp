#include "myTime.h"

myTime::myTime() {
    _timestamp = 0;
    _timezone = "UTC";
    _ntpServer = "pool.ntp.org";
    _updateInterval = 3600000; // 1 hour
    _lastUpdate = 0;
    _offsetHours = 0.0;
}

void myTime::parseTimeZone(const String& tz) {
    int plusIndex = tz.indexOf('+');
    int minusIndex = tz.indexOf('-');
    int index = (plusIndex > 0) ? plusIndex : minusIndex;
    
    if (index > 0) {
        // Split IANA and offset
        _timezone = tz.substring(0, index);
        String offsetStr = tz.substring(index);
        // parse hours and optional minutes
        int colon = offsetStr.indexOf(':');
        if (colon >= 0) {
            float hours = offsetStr.substring(0, colon).toFloat();
            float minutes = offsetStr.substring(colon + 1).toFloat();
            _offsetHours = (hours >= 0 ? 1 : -1) * (abs(hours) + minutes/60.0);
        } else {
            _offsetHours = offsetStr.toFloat();
        }
    } else {
        _timezone = tz;
        _offsetHours = 0.0;
    }
}

void myTime::setTimeZone(const String& tz) {
    parseTimeZone(tz);
}

void myTime::setNtpServer(const String& server) {
    _ntpServer = server;
}

void myTime::setUpdateInterval(unsigned long intervalMs) {
    _updateInterval = intervalMs;
}

void myTime::updateFromNTP() {
    Serial.println("Contacting NTP server...");
    configTzTime(_timezone.c_str(), _ntpServer.c_str());
    
    struct tm timeinfo;
    int retries = 30;  // Increased retries
    bool success = false;
    
    while (retries > 0) {
        if (getLocalTime(&timeinfo)) {
            time_t tempTime = mktime(&timeinfo);
            // Check if we got a valid time (not epoch 0 = 1970)
            if (tempTime > 946684800) {  // Jan 1, 2000 as minimum valid time
                _timestamp = tempTime;
                // apply optional offset
                if (_offsetHours != 0.0) {
                    _timestamp += (time_t)(_offsetHours * 3600);
                }
                _lastUpdate = millis();
                success = true;
                Serial.println("Time synchronized successfully!");
                break;
            }
        }
        Serial.print(".");
        delay(500);  // Increased delay
        retries--;
    }
    
    if (!success) {
        Serial.println("\nFailed to get valid NTP time after all retries");
    }
}

void myTime::loopUpdate() {
    if (millis() - _lastUpdate >= _updateInterval) {
        updateFromNTP();
    }
}

int myTime::Y() {
    struct tm* t = localtime(&_timestamp);
    return t->tm_year + 1900;
}

int myTime::Mon() {
    struct tm* t = localtime(&_timestamp);
    return t->tm_mon + 1;
}

int myTime::D() {
    struct tm* t = localtime(&_timestamp);
    return t->tm_mday;
}

int myTime::H() {
    struct tm* t = localtime(&_timestamp);
    return t->tm_hour;
}

int myTime::M() {
    struct tm* t = localtime(&_timestamp);
    return t->tm_min;
}

int myTime::S() {
    struct tm* t = localtime(&_timestamp);
    return t->tm_sec;
}

time_t myTime::timestamp() {
    return _timestamp;
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
