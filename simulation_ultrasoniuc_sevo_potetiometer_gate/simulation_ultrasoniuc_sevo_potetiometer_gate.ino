#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2); // I2C address 0x20, 16x2 LCD
Servo myservo;

// Pins for ultrasonic sensor and LEDs
#define trigPin 1
#define echoPin 4
#define redLedPin 2
#define greenLedPin 3

// Variable resistor and manual override
#define variableResistorPin A0
#define ExitButtonPin 9

// Parking system variables
const int totalSpaces = 5;
int freeSpaces = totalSpaces;
int parkedCars = 0;

// Ultrasonic sensor variables
long duration;

// Initialize the system
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(ExitButtonPin, INPUT_PULLUP);

  myservo.attach(7);
  lcd.begin(16, 2);

  lcd.print("Parking System");
  delay(2000);
  lcd.clear();
  updateLCD();
}

void loop() {
  int distance = measureDistance();
  int manualOverride = analogRead(variableResistorPin);

  // Handle car entry
  if (freeSpaces > 0 && distance > 0 && distance < 50) {
    handleCarEntry();
  }

  // Handle car exit
  if (digitalRead(ExitButtonPin) == LOW && freeSpaces < totalSpaces) {
    handleCarExit();
  }

  // Update LEDs and display
  updateLEDs();
  updateLCD();

  delay(100);
}

// Handle car entry
void handleCarEntry() {
  freeSpaces--;
  parkedCars++;
  lcd.clear();
  lcd.print("Car Entered");
  lcd.setCursor(0, 1);
  lcd.print("Free: ");
  lcd.print(freeSpaces);
  delay(2000);

  openGate();
  delay(3000);
  closeGate();
}

// Handle car exit
void handleCarExit() {
  freeSpaces++;
  parkedCars--;
  lcd.clear();
  lcd.print("Car Exited");
  lcd.setCursor(0, 1);
  lcd.print("Free: ");
  lcd.print(freeSpaces);
  delay(2000);

  openGate();
  delay(3000);
  closeGate();
}

// Measure distance using ultrasonic sensor
int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}

// Open gate
void openGate() {
  for (int pos = 0; pos <= 90; pos++) {
    myservo.write(pos);
    delay(15);
  }
}

// Close gate
void closeGate() {
  for (int pos = 90; pos >= 0; pos--) {
    myservo.write(pos);
    delay(15);
  }
}

// Update LEDs based on parking availability
void updateLEDs() {
  if (freeSpaces == 0) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
  }
}

// Update LCD display
void updateLCD() {
  lcd.clear();
  lcd.print("Free: ");
  lcd.print(freeSpaces);
  lcd.print("/");
  lcd.print(totalSpaces);
  lcd.setCursor(0, 1);
  lcd.print("Parked: ");
  lcd.print(parkedCars);
}