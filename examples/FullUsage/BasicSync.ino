/*
 * BasicSync - Simple NTP time synchronization
 * 
 * This example shows the most basic usage of myTime:
 * - Connect to WiFi
 * - Sync with NTP server
 * - Display the current time
 */

#include <WiFi.h>
#include <myTime.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

myTime myClock;

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== myTime Basic Sync Example ===");
    
    // Connect to WiFi
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    
    // Sync time from NTP (defaults to UTC and pool.ntp.org)
    Serial.println("Syncing with NTP server...");
    myClock.updateFromNTP();
    
    Serial.println("Time synchronized!");
    Serial.print("Current UTC time: ");
    Serial.println(myClock.formattedDateTime());
}

void loop() {
    // Display time every second
    Serial.println(myClock.formatted());
    delay(1000);
}
