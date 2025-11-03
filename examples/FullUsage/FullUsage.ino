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
