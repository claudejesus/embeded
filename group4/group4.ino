#include <Wire.h>
#include <RTClib.h>  // Library for DS3231 RTC
#include <Servo.h>   // Library for controlling the servo motor
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

// Ultrasonic Sensor Pins
const int trigPin = 7; // Trigger pin
const int echoPin = 6; // Echo pin

// Servo Motor Pin
Servo myServo;
const int servoPin = 9;

// RTC Object
RTC_DS3231 rtc;

// Threshold distances for sorting (in cm)
const int smallObject = 5;  // Small object: less than 5 cm
const int mediumObject = 10; // Medium object: 5–10 cm
const int largeObject = 15;  // Large object: greater than 10 cm

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize Ultrasonic Sensor Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize Servo Motor
  myServo.attach(servoPin);
  myServo.write(90); // Initial position of the servo

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to current date/time
  }
}

void loop() {
  DateTime now = rtc.now(); // Get the current time from RTC
  Serial.print("Current Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.println(now.minute());

  // Example: System operates between 8:00 AM and 6:00 PM
  if (now.hour() >= 8 && now.hour() < 18) {
    Serial.println("System Active");

    // Measure distance using Ultrasonic Sensor
    long distance = measureDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Sort objects based on size
    if (distance < smallObject) {
      sortObject(0); // Bin 0 for small objects
    } else if (distance >= smallObject && distance <= mediumObject) {
      sortObject(1); // Bin 1 for medium objects
    } else if (distance > mediumObject) {
      sortObject(2); // Bin 2 for large objects
    }
  } else {
    Serial.println("System Inactive - Outside Working Hours");
  }
  delay(2000); // 2-second delay between readings
}

// Function to measure distance using the Ultrasonic Sensor
long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Convert duration to cm
  return distance;
}

// Function to control the Servo Motor for sorting
void sortObject(int bin) {
  switch (bin) {
    case 0:
      myServo.write(45); // Small object bin
      Serial.println("Small Object Detected - Moving to Bin 0");
      break;
    case 1:
      myServo.write(90); // Medium object bin
      Serial.println("Medium Object Detected - Moving to Bin 1");
      break;
    case 2:
      myServo.write(135); // Large object bin
      Serial.println("Large Object Detected - Moving to Bin 2");
      break;
  }
  delay(1000); // Allow time for servo to move
  myServo.write(90); // Reset servo to default position
}
