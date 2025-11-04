#include <WiFi.h>
#include <myTime.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

myTime clock;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected!");

  clock.setNtpServer("pool.ntp.org");
  clock.setUpdateInterval(600000); // 10 minutes
  clock.updateFromNTP();

  Serial.println("=== World Clock ===");
}

void loop() {
  Serial.println();
  for (int i = 0; i < 6; i++) {  // Show first 6 zones for demo
    clock.setTimeZone(IANATimeZones[i]);
    clock.updateFromNTP();
    Serial.printf("%-15s (%s): %s\n", 
      IANATimeZoneCities[i],
      IANATimeZones[i],
      clock.formattedDateTime().c_str());
    delay(1000);
  }
  delay(10000);
}
