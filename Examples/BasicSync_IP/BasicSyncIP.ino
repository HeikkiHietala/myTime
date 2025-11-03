/*
 * BasicSync_IP - Simple NTP time synchronization using IP
 * 
 * This example:
 * - Connects to WiFi
 * - Syncs time using direct IP to NTP server
 * - Displays current time every second
 */

#include <WiFi.h>
#include <myTime.h>

const char* ssid = "YourHotspotName";      // Replace with your WiFi SSID
const char* password = "YourHotspotPassword";  // Replace with your WiFi password

myTime myClock;

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== myTime Basic Sync via IP ===");

    // Connect to WiFi
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    // Use direct IP to avoid DNS issues
    myClock.setNtpServer("129.6.15.28"); // time.nist.gov

    // Use fixed UTC offset (e.g. Helsinki UTC+3 = 10800 seconds)
    myClock.setFixedOffset(10800);

    // Sync time
    if (myClock.updateFromNTP()) {
        Serial.println("✓ Time synchronized!");
        Serial.println(myClock.formattedDateTime());
    } else {
        Serial.println("✗ Time sync failed");
    }
}

void loop() {
    Serial.println(myClock.formatted());
    delay(1000);
}
``
