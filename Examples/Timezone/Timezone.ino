/*
 * TimezoneDST - Demonstrates NTP sync with timezone and DST handling
 *
 * Features:
 * - Connects to WiFi
 * - Syncs time from NTP server using direct IP
 * - Applies IANA timezone (Europe/Helsinki)
 * - Automatically adjusts for DST based on rules in myTime library
 */

#include <WiFi.h>
#include <myTime.h>

const char* ssid = "YOUR-SSID";      // Replace with your WiFi SSID
const char* password = "YOUR-PASSWORD*";  // Replace with your WiFi password
myTime myClock;

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== Timezone + DST Example ===");

    // Connect to WiFi
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    // Use direct IP for NTP to avoid DNS issues
    myClock.setNtpServer("129.6.15.28"); // time.nist.gov

    // Set timezone (DST rules are applied automatically for Europe)
    myClock.setTimeZone("Europe/Helsinki");

    // Sync time
    if (myClock.updateFromNTP()) {
        Serial.println("✓ Time synchronized!");
        Serial.print("Current local time: ");
        Serial.println(myClock.formattedDateTime());

        // Show DST status (based on internal logic)
        time_t nowUTC = time(nullptr);
        bool dstActive = (myClock.formattedDateTime().indexOf("DST") >= 0); // Placeholder
        Serial.print("DST active? ");
        Serial.println(dstActive ? "Yes" : "No");
    } else {
        Serial.println("✗ Time sync failed");
    }
}

void loop() {
    Serial.println(myClock.formattedDateTime());
    delay(1000);
}
