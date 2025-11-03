#include "myTime.h"
#include <lwip/apps/sntp.h>

myTime::myTime() {
    _timezone = "UTC";
    _ntpServer = "pool.ntp.org";
    _updateInterval = 3600000;
    _lastUpdate = 0;
    _offsetSeconds = 0;
    _ntpInitialized = false;
    _useFixedOffset = false;
}

void myTime::initializeNTP() {
    if (!_ntpInitialized) {
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, _ntpServer.c_str());
        sntp_init();
        _ntpInitialized = true;
    }
}

void myTime::setTimeZone(const String& tz) {
    _timezone = tz;
    _useFixedOffset = false;
    time_t now = time(nullptr);
    if (now > 946684800) {
        _offsetSeconds = calculateTimezoneOffset(_timezone, now);
    }
}

void myTime::setFixedOffset(int seconds) {
    _offsetSeconds = seconds;
    _useFixedOffset = true;
}

void myTime::setNtpServer(const String& server) {
    if (_ntpInitialized) {
        sntp_stop();
        _ntpInitialized = false;
    }
    _ntpServer = server;
}

void myTime::setUpdateInterval(unsigned long intervalMs) {
    _updateInterval = intervalMs;
}

bool myTime::updateFromNTP() {
    initializeNTP();
    int retries = 20;
    time_t now;

    while (retries-- > 0) {
        now = time(nullptr);
        if (now > 946684800) {
            _lastUpdate = millis();
            if (!_useFixedOffset) {
                _offsetSeconds = calculateTimezoneOffset(_timezone, now);
            }
            return true;
        }
        delay(500);
    }
    return false;
}

void myTime::loopUpdate() {
    if (millis() - _lastUpdate >= _updateInterval) {
        updateFromNTP();
    }
}

int myTime::calculateTimezoneOffset(const String& tz, time_t utcTime) {
    String tzName = tz;
    tzName.toLowerCase();

    if (tzName == "utc") return 0;
    if (tzName == "europe/helsinki") return 7200 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "europe/london") return 0 + (isDST(tz, utcTime) ? 3600 : 0);
    if (tzName == "america/new_york") return -18000 + (isDST(tz, utcTime) ? 3600 : 0);
    return 0;
}

bool myTime::isDST(const String& tz, time_t utcTime) {
    struct tm* timeinfo = gmtime(&utcTime);
    int month = timeinfo->tm_mon + 1;
    int day = timeinfo->tm_mday;

    String tzName = tz;
    tzName.toLowerCase();

    if (tzName.startsWith("europe/")) {
        if (month > 3 && month < 10) return true;
        if (month < 3 || month > 10) return false;
        if (month == 3) return day >= 25;
        if (month == 10) return day < 25;
    }

    return false;
}

int myTime::Y() { time_t now = time(nullptr) + _offsetSeconds; return gmtime(&now)->tm_year + 1900; }
int myTime::Mon() { time_t now = time(nullptr) + _offsetSeconds; return gmtime(&now)->tm_mon + 1; }
int myTime::D() { time_t now = time(nullptr) + _offsetSeconds; return gmtime(&now)->tm_mday; }
int myTime::H() { time_t now = time(nullptr) + _offsetSeconds; return gmtime(&now)->tm_hour; }
int myTime::M() { time_t now = time(nullptr) + _offsetSeconds; return gmtime(&now)->tm_min; }
int myTime::S() { time_t now = time(nullptr) + _offsetSeconds; return gmtime(&now)->tm_sec; }

time_t myTime::timestamp() { return time(nullptr) + _offsetSeconds; }

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
