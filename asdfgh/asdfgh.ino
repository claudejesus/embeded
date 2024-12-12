
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Define pin connections
#define ENTRY_BUTTON 8
#define EXIT_BUTTON 9
#define GREEN_LED 3
#define RED_LED 2
#define TRIG_PIN 6
#define ECHO_PIN 5

// Parking spaces and variables
int totalSpaces = 10; 
int freeSpaces = 10;  

// Function declarations
void updateDisplay();
void updateStatus();
int measureDistance();
void updateLEDs();

void setup() {
  pinMode(ENTRY_BUTTON, INPUT_PULLUP);
  pinMode(EXIT_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Green LED ON initially
  digitalWrite(GREEN_LED, HIGH); 
  digitalWrite(RED_LED, LOW);   

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Welcome message
  lcd.setCursor(0, 0);
  lcd.print("Welcome to Goico");
  Serial.begin(9600);
  Serial.println("Welcome to Goico");
  lcd.setCursor(0, 1);
  lcd.print("Parking System");
  Serial.println("Parking System");

  delay(2000); // Wait for 2 seconds to show the welcome message
  
  // Initial free spaces display
  updateDisplay();
}

void loop() {
  // Entry button logic
  if (digitalRead(ENTRY_BUTTON) == LOW) {
    delay(200); // Debouncing
    if (freeSpaces > 0) {
      freeSpaces--;
      updateStatus(); 
    }
  }

  // Exit button logic
  if (digitalRead(EXIT_BUTTON) == LOW) {
    delay(200); // Debouncing
    if (freeSpaces < totalSpaces) {
      freeSpaces++;
      updateStatus();
    }
  }

  // Ultrasonic sensor logic for range detection
  int distance = measureDistance();
  if (distance > 10 && distance <= 200) { // Check range between 10 cm and 2 meters
    if (freeSpaces > 0) {
      freeSpaces--;
      updateStatus();
      delay(1000); // Simulate car parking
    }
  } else if (distance > 200) {
    Serial.println("No vehicle detected within range");
  }
}

// Function to measure distance using the ultrasonic sensor
int measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2; // Convert to cm
  Serial.println(distance);
  return distance;
}

// Function to update LEDs based on free spaces
void updateLEDs() {
  if (freeSpaces){}}