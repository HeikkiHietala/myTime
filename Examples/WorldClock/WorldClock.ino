#include <WiFi.h>
#include <myTime.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

myTime myClock;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected!");

  myClock.setNtpServer("pool.ntp.org");
  myClock.setUpdateInterval(600000); // 10 minutes
  myClock.updateFromNTP();

  Serial.println("=== World Clock ===");
}

void loop() {
  Serial.println();
  for (int i = 0; i < 6; i++) {  // Show first 6 zones for demo
    myClock.setTimeZone(IANATimeZones[i]);
    myClock.updateFromNTP();
    Serial.printf("%-15s (%s): %s\n", 
      IANATimeZoneCities[i],
      IANATimeZones[i],
      myClock.formattedDateTime().c_str());
    delay(1000);
  }
  delay(10000);
}
