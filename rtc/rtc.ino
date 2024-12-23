#include <Wire.h>
#include <RTClib.h>

// RTC Object
RTC_DS3231 rtc;

// LED Pin
const int ledPin = 9;

void setup() {
  pinMode(ledPin, OUTPUT); // Set LED pin as OUTPUT
  Serial.begin(9600); // Initialize serial monitor

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Set the RTC time (only needed if RTC is not set or lost power)
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Uncomment to set to compile time
}

void loop() {
  // Get current time
  DateTime now = rtc.now();

  // Print current time to Serial Monitor
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.println(now.second());

  // LED ON at 6:00 AM and OFF at 8:00 PM
  if (now.hour() >= 6 && now.hour() < 20) {
    digitalWrite(ledPin, HIGH); // Turn LED ON
  } else {
    digitalWrite(ledPin, LOW); // Turn LED OFF
  }

  delay(1000); // Update every second
}
