#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Ultrasonic Sensor Pins
const int trigPin = 7; // Trigger pin
const int echoPin = 6; // Echo pin

#define s3 3
#define s1 4

const int analog1 = s1; 
const int analog2 = s3;

// Servo Motor Pin
Servo myServo;
const int servoPin = 9;


// RTC Object
RTC_DS3231 rtc;

// LCD Object
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjusted I2C address

// Threshold distances for sorting (in cm)
const int smallObject = 5;
const int mediumObject = 10;
const int largeObject = 15;

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
    // Wire.begin(s1, s3);
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        lcd.print("Couldn't find RTC");
        while (1);
    }
    if (rtc.lostPower()) {
        rtc.adjust(DateTime(2024, 12, 20, 12, 0, 0)); // Set RTC to current date/time
    }

    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sorting System");
    delay(2000);
    lcd.clear();
}

void loop() {
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.print(now.hour());
    lcd.print(":");
    lcd.print(now.minute());

    if (now.hour() >= 8 && now.hour() < 18) {
        lcd.setCursor(0, 1);
        lcd.print("System Active  ");
        Serial.println("System Active");

        long distance = measureDistance();
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        lcd.setCursor(0, 1);
        lcd.print("Distance: ");
        lcd.print(distance);
        lcd.print(" cm  ");

        if (distance < smallObject) {
            sortObject(0);
        } else if (distance >= smallObject && distance <= mediumObject) {
            sortObject(1);
        } else if (distance > mediumObject) {
            sortObject(2);
        }
    } else {
        lcd.setCursor(0, 1);
        lcd.print("System Inactive ");
        Serial.println("System Inactive - Outside Working Hours");
    }
    delay(2000);
}

long measureDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    long distance = duration * 0.034 / 2;
    return distance;
}

void sortObject(int bin) {
    switch (bin) {
        case 0:
            myServo.write(45);
            Serial.println("Small Object Detected - Moving to Bin 0");
            lcd.setCursor(0, 1);
            lcd.print("Small Obj -> Bin 0");
            break;
        case 1:
            myServo.write(90);
            Serial.println("Medium Object Detected - Moving to Bin 1");
            lcd.setCursor(0, 1);
            lcd.print("Medium Obj -> Bin1");
            break;
        case 2:
            myServo.write(135);
            Serial.println("Large Object Detected - Moving to Bin 2");
            lcd.setCursor(0, 1);
            lcd.print("Large Obj -> Bin 2");
            break;
    }
    delay(1000);
    myServo.write(90);
}