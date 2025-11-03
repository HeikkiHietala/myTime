#include <WiFi.h>
#include <myTime.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

myTime myClock;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change address if needed (0x27 or 0x3F)

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Connecting WiFi");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    lcd.clear();
    lcd.print("WiFi Connected");

    myClock.setNtpServer("129.6.15.28"); // time.nist.gov
    myClock.setTimeZone("Europe/Helsinki"); // DST handled automatically

    if (myClock.updateFromNTP()) {
        lcd.clear();
        lcd.print("Time Synced!");
        delay(1000);
    } else {
        lcd.clear();
        lcd.print("Sync Failed");
        delay(2000);
    }
}

void loop() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(myClock.formattedDateTime().substring(0, 10)); // YYYY-MM-DD
    lcd.setCursor(0, 1);
    lcd.print(myClock.formatted()); // HH:MM:SS
    delay(1000);
}
