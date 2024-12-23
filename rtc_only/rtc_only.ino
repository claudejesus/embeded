#include <Wire.h>  // Standard Wire library
#include <RTClib.h>  // RTC Library for DS1307
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x20, 16, 2);

RTC_DS1307 rtc;

// Define I2C pins using D1 and D2
#define sda  21  // D1 pin on ESP32 (GPIO21)
#define scl  22  // D2 pin on ESP32 (GPIO22)
// int sda = 21;
// int scl = 22;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.println("welcome");

  // Initialize I2C with custom SDA and SCL pins
wire.begin(sda, scl);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Check connections!");
    lcd.print("connections!");
    while (1);
  }

  // Check if RTC is running
  if (!rtc.isrunning()) {
    Serial.println("RTC is not running, setting the time...");
    lcd.print("RTC is not running, setting the time...");
    // Set the RTC to the current time (use the compile time of this sketch)
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("RTC Initialized");
}

void loop() {
  DateTime now = rtc.now(); // Fetch current time from RTC

  // Print the current time to the Serial Monitor
  Serial.print(now.year());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.print(now.second());
  Serial.println();
  delay(1000); // 1-second delay
}
