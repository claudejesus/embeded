#include <Wire.h>
#include <RTClib.h>  // Library for DS3231 RTC
#include <Servo.h>   // Library for controlling the servo motor
#include <LiquidCrystal_I2C.h> // Library for I2C-based 16x2 LCD display

// Ultrasonic Sensor Pins
const int trigPin = 7; // Trigger pin
const int echoPin = 6; // Echo pin

// Servo Motor Pin
Servo myServo;
const int servoPin = 9;

// RTC Object
RTC_DS3231 rtc;

// LCD Object (I2C Address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Threshold distances for sorting (in cm)
const int smallObject = 5;  // Small object: less than 5 cm
const int mediumObject = 10; // Medium object: 5–10 cm
const int largeObject = 15;  // Large object: greater than 10 cm

// Bin capacities
int smallBinCapacity = 10;
int mediumBinCapacity = 10;
int largeBinCapacity = 10;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  lcd.setCursor(0, 1);
  lcd.print("AUTOMATION SORTING");

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
    lcd.print("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to current date/time
  }

  delay(2000);
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now(); // Get the current time from RTC
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());

  // Example: System operates between 8:00 AM and 6:00 PM
  if (now.hour() >= 00 && now.hour() < 24) {
    lcd.setCursor(0, 1);
    lcd.print("System Active   ");
    Serial.println("System Active");

    // Measure distance using Ultrasonic Sensor
    long distance = measureDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Display the distance on LCD
    lcd.setCursor(0, 1);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm ");

    // Sort objects based on size
    if (distance < smallObject) {
      if (smallBinCapacity > 0) {
        sortObject(0);
        smallBinCapacity--;
        Serial.print("Small Bin Remaining: ");
        Serial.println(smallBinCapacity);
        lcd.setCursor(0, 1);
        lcd.print("Small: ");
        lcd.print("Used: ");
        lcd.print(10 - smallBinCapacity);
        // lcd.print(" Avl: ");
        lcd.print("/");
        lcd.print(smallBinCapacity);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Small Full ");
        // lcd.print(smallBinCapacity);
        lcd.print(distance);
        Serial.println("Small Bin Full");
      }
    } else if (distance >= smallObject && distance <= mediumObject) {
      if (mediumBinCapacity > 0) {
        sortObject(1);
        mediumBinCapacity--;
        Serial.print("Medium Remaining: ");
        Serial.println(mediumBinCapacity);
        lcd.setCursor(0, 1);
        lcd.print("Medium: ");
        lcd.print("Used: ");
        lcd.print(10 - mediumBinCapacity);
        // lcd.print(" Avl: ");
        lcd.print("/");
        lcd.print(mediumBinCapacity);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Medium Full");
        // lcd.print(mediumBinCapacity);
        lcd.print(distance);
        Serial.println("Medium Bin Full");
      }
    } else if (distance > mediumObject) {
      if (largeBinCapacity > 0) {
        sortObject(2);
        largeBinCapacity--;
        Serial.print("Large Remaining: ");
        Serial.println(largeBinCapacity);
        lcd.setCursor(0, 1);
        lcd.print("Large: ");
        lcd.print("Used: ");
        lcd.print(10 - largeBinCapacity);
        lcd.print("/");
        lcd.print(largeBinCapacity);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Large Full ");
        lcd.print(distance);
        
        Serial.println("Large Bin Full");
      }
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("System Inactive  ");
    lcd.setCursor(0, 1);
    lcd.print("Outside Hours ");
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
      lcd.setCursor(0, 1);
      lcd.print("Sorting: SmallObj");
      break;
    case 1:
      myServo.write(90); // Medium object bin
      Serial.println("Medium Object Detected - Moving to Bin 1");
      lcd.setCursor(0, 1);
      lcd.print("Sorting: MediumObj");
      break;
    case 2:
      myServo.write(135); // Large object bin
      Serial.println("Large Object Detected - Moving to Bin 2");
      lcd.setCursor(0, 1);
      lcd.print("Sorting: LargeObj");
      break;
  }
  delay(1000); // Allow time for servo to move
  myServo.write(90); // Reset servo to default position
}
