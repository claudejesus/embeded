#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Pin Definitions
const int ultrasonicTrigPin = 6;  // Ultrasonic Trigger Pin
const int ultrasonicEchoPin = 5; // Ultrasonic Echo Pin
const int greenLED = 3;           // Green LED Pin (Spaces Available)
const int redLED = 2;             // Red LED Pin (Parking Full)
const int servoPin = 10;           // Servo Motor Pin
const int variableResistorPin = A0;  // Variable Resistor Pin for manual gate control

// LCD Setup
LiquidCrystal_I2C lcd(0x20, 16, 2);  // Adjust as per your wiring

// Variables
int totalSpaces = 10;       // Total parking spaces
int freeSpaces = totalSpaces; // Free spaces
int parkedCars = 0;         // Number of cars parked
int carHeight = 0;          // Car height detected by ultrasonic sensor
int maxHeight = 100;        // Maximum height threshold for a car (example value)

// Servo setup
Servo parkingGate;

void setup() {
  Serial.begin(9600);// Initialize LCD
  Serial.write("Parking System");
  digitalWrite(greenLED, HIGH);
  lcd.begin(16, 2);
  lcd.print("Parking System");

  // Initialize Pins
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(ultrasonicTrigPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);
  
  // Initialize Servo
  parkingGate.attach(servoPin);
  
  // Initial Gate State (Open)
  parkingGate.write(0); // Gate is open at the start
  
  // Initial LED States
  digitalWrite(greenLED, HIGH); // Green LED lights up (space available)
  digitalWrite(redLED, LOW);    // Red LED off initially
}

void loop() {
  // Check if there is space available for entry
  if (freeSpaces > 0) {
    digitalWrite(greenLED, HIGH); // Turn on Green LED (space available)
    digitalWrite(redLED, LOW);    // Turn off Red LED
  } else {
    digitalWrite(greenLED, LOW);  // Turn off Green LED
    digitalWrite(redLED, HIGH);   // Turn on Red LED (no spaces available)
  }
  
  // Ultrasonic Sensor: Detecting Car Entry
  carHeight = readUltrasonicDistance();
  
  // If a car is detected (height value above the threshold)
  if (carHeight > 0 && carHeight < maxHeight) {
    // Simulate Car Entry (Decrease free spaces, Increase parked cars)
    if (freeSpaces > 0) {
      freeSpaces--;
      parkedCars++;
      
      // Update LCD Display
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Free Spaces: ");
      lcd.print(freeSpaces);
      lcd.setCursor(0, 1);
      lcd.print("Parked Cars: ");
      lcd.print(parkedCars);
      
      // Delay for debounce
      delay(1000);
    }
  }
  
  // If parking is full, automatically close the gate
  if (freeSpaces == 0) {
    parkingGate.write(90); // Close the gate
  }
  
  // Manual Control for Gate via Variable Resistor
  int manualControl = analogRead(variableResistorPin); // Read value from the variable resistor (0 to 1023)
  
  // If variable resistor is below a threshold, close the gate
  if (manualControl < 512) {
    parkingGate.write(90); // Close the gate
  } else {
    if (freeSpaces > 0) {
      parkingGate.write(0); // Open the gate if there is space
    }
  }
  
  delay(500); // Wait before checking again
}

// Function to read the ultrasonic sensor distance
long readUltrasonicDistance() {
  digitalWrite(ultrasonicTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrigPin, LOW);
  
  long duration = pulseIn(ultrasonicEchoPin, HIGH);
  long distance = duration * 0.034 / 2; // Calculate the distance in cm
  
  return distance; // Return the car height or distance detected
}
