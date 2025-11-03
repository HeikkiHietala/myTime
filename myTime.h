#ifndef MYTIME_H
#define MYTIME_H

#include <Arduino.h>
#include <time.h>
#include <sys/time.h>

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
    void setFixedOffset(int seconds); // NEW

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

#endif
