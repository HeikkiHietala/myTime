#include "myTime.h"

myTime::myTime() {
    _timezone = "UTC";
    _ntpServer = "pool.ntp.org";
    _updateInterval = 3600000; // 1 hour default
    _lastUpdate = 0;
    _offsetSeconds = 0;
    _ntpInitialized = false;
    _useFixedOffset = false;
}

void myTime::setTimeZone(const String& tz) {
    _timezone = tz;
    _useFixedOffset = false;
    _ntpInitialized = false;
}

void myTime::setNtpServer(const String& server) {
    _ntpServer = server;
    _ntpInitialized = false;
}

void myTime::setUpdateInterval(unsigned long intervalMs) {
    _updateInterval = intervalMs;
}

void myTime::setFixedOffset(int seconds) {
    _offsetSeconds = seconds;
    _useFixedOffset = true;
}

void myTime::initializeNTP() {
    if (_useFixedOffset) {
        configTime(_offsetSeconds, 0, _ntpServer.c_str());
    } else {
        configTzTime(_timezone.c_str(), _ntpServer.c_str());
    }
    _ntpInitialized = true;
}

bool myTime::updateFromNTP() {
    if (!_ntpInitialized) initializeNTP();

    struct tm timeinfo;
    int retry = 0;
    const int retry_count = 10;
    while (!getLocalTime(&timeinfo) && retry < retry_count) {
        delay(500);
        retry++;
    }
    if (retry == retry_count) return false;

    _lastUpdate = millis();
    return true;
}

void myTime::loopUpdate() {
    if (millis() - _lastUpdate > _updateInterval) {
        updateFromNTP();
    }
}

time_t myTime::timestamp() {
    time_t now;
    time(&now);
    return now;
}

int myTime::Y() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo.tm_year + 1900;
}

int myTime::Mon() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo.tm_mon + 1;
}

int myTime::D() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo.tm_mday;
}

int myTime::H() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo.tm_hour;
}

int myTime::M() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo.tm_min;
}

int myTime::S() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo.tm_sec;
}

String myTime::formatted() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    char buffer[9];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
    return String(buffer);
}

String myTime::formattedDateTime() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
}
