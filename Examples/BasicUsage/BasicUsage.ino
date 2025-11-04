#include <WiFi.h>
#include <myTime.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

myTime clock;

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
  clock.setTimeZone("Europe/Helsinki");    // Use IANA timezone
  clock.setNtpServer("pool.ntp.org");      // Default NTP server
  clock.setUpdateInterval(60000);          // Update every 60 seconds

  if (clock.updateFromNTP()) {
    Serial.println("Time synchronized successfully!");
  } else {
    Serial.println("NTP sync failed!");
  }

  Serial.println(clock.formattedDateTime());
}

void loop() {
  clock.loopUpdate();  // handles periodic NTP refresh
  Serial.println(clock.formattedDateTime());
  delay(1000);
}
