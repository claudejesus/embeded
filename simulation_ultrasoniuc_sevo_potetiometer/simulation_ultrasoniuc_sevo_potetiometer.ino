#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

// Define Pins
#define ENTRY_BUTTON 8
#define EXIT_BUTTON 9
#define GREEN_LED 3
#define RED_LED 2
#define TRIG_PIN 6
#define ECHO_PIN 5
Servo myservo;  
int pos = 0;   
#define variableResistorPin A0 // Analog pin for variable resistor

int totalSpaces = 10;
int freeSpaces = 10;
const int gateOpen = 90;   // Open position
const int gateClosed = 0;  // Closed position


bool lastEntryState = HIGH;
bool lastExitButtonState = HIGH;

void setup() {
  Serial.begin (9600);
  // Set up pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(EXIT_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(variableResistorPin, INPUT);

  // pinMode(GND1, OUMode(GND2, OUTPUT);

  // Green LED ON initially
  digitalWrite(GREEN_LED, HIGH);

  // Red LED OFF initially
  digitalWrite(RED_LED, LOW);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Display welcome message
  lcd.setCursor(0, 0);
  lcd.print("Welcome to Goico");
  lcd.setCursor(0, 1);
  lcd.print("Parking System");
  delay(2000);

  // Display the initial number of free spaces
  updateDisplay();
}

void loop() {
  long duration, distance;
  digitalWrite(TRIG_PIN, HIGH);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(10); // Added this line
  digitalWrite(TRIG_PIN, HIGH);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 2;
  distance = duration * 0.034 / 2;

  // Check if distance is within car detection range
  //return (distance > 0 && distance < 30);
  bool currentEntryState = digitalRead(TRIG_PIN);
  //bool lastEntryState = digitalRead();
  // Ultrasonic sensor logic
  if (currentEntryState && lastEntryState == HIGH) {
    if (distance < 50){
      delay(200); // Debouncing
    if (freeSpaces > 0) {
      freeSpaces--;
      updateStatus(); // Update LEDs and display
    }
    }
  }
  
  bool currentExitButtonState = digitalRead(EXIT_BUTTON);
  // Exit Button Logic
  if (currentExitButtonState == LOW && lastExitButtonState == HIGH) {
    delay(100); // Debouncing
    if (freeSpaces < totalSpaces) {
      freeSpaces++;
      updateStatus(); // Update LEDs and display
    }
  }
  lastEntryState = currentEntryState;
  lastExitButtonState = currentExitButtonState;
}

//& distance < 30); // Adjust range based on your setup
// }

// Function to update LEDs based on free spaces
void updateLEDs() {
  if (freeSpaces == 0) {
    digitalWrite(GREEN_LED, LOW);  // No space available
    digitalWrite(RED_LED, HIGH);  // Red LED ON
  } else {
    digitalWrite(GREEN_LED, HIGH); // Space available
    digitalWrite(RED_LED, LOW);   // Red LED OFF
  }
}

// Function to update the LCD display
void updateDisplay() {

  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0);
  lcd.print("Spaces Avail: ");
  lcd.print(freeSpaces); // Display the number of free spaces
  Serial.print("Spaces Avail: ");
  Serial.print(freeSpaces);
  Serial.println("Distance: ");
  // Serial.print(distance);

  // Show parking status
  lcd.setCursor(0, 1);
  if (freeSpaces == 0) {
    lcd.print("Status: FULL");
    Serial.println("Status: FULL");
  } else {
    lcd.print("Status: OPEN");
    Serial.println("Status: OPEN");
  }
}

// Function to update both LEDs and LCD
void updateStatus() {
  updateLEDs();    // Update LEDs based on free spaces
  updateDisplay(); // Update LCD with the current status
}