#include <WiFi.h>
#include <myTime.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

myTime myClock;

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // Configure clock
  myClock.setTimeZone("Europe/Helsinki");    // Use IANA timezone
  myClock.setNtpServer("pool.ntp.org");      // Default NTP server
  myClock.setUpdateInterval(60000);          // Update every 60 seconds

  if (myClock.updateFromNTP()) {
    Serial.println("Time synchronized successfully!");
  } else {
    Serial.println("NTP sync failed!");
  }

  Serial.println(myClock.formattedDateTime());
}

void loop() {
  myClock.loopUpdate();  // handles periodic NTP refresh
  Serial.println(myClock.formattedDateTime());
  delay(1000);
}
