#include <Wire.h>
#include <LiquidCrystal.h>

// LCD pin connections
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Pin definitions
#define TRIG_PIN 8
#define ECHO_PIN 9
#define LED_PIN 12

// Gate monitoring buttons
#define GATE1_BUTTON 10 // Gate 1 attendee counter
#define GATE2_BUTTON 13 // Gate 2 attendee counter

// Variables
int distanceThreshold = 500;   // Threshold for car detection (in cm)
int parkingSlots = 0;          // Tracks number of cars in the parking
int maxParkingSlots = 6;      // Maximum parking slots available
bool carDetected = false;      // Tracks if a car is currently detected

int gate1Attendees = 0;        // Attendees entering through Gate 1
int gate2Attendees = 0;        // Attendees entering through Gate 2
int totalAttendees = 0;        // Total attendees from both gates

void setup() {
  // Initialize serial and LCD
  Serial.begin(9600);
  lcd.begin(16, 2);

  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("Smart Entry");
  lcd.setCursor(0, 1);
  lcd.print("System Loading...");
  delay(2000);
  lcd.clear();

  // Pin setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(GATE1_BUTTON, INPUT_PULLUP);
  pinMode(GATE2_BUTTON, INPUT_PULLUP);

  digitalWrite(LED_PIN, LOW); // Turn off the LED initially
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

void loop() {
  // Car monitoring
  int distance = getDistance();
  Serial.print("Car Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < distanceThreshold && !carDetected && parkingSlots < maxParkingSlots) {
    carDetected = true; // Detect a car
    parkingSlots++;     // Increment parking slot count

    // Display car entry
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Car Entered!");
    lcd.setCursor(0, 1);
    lcd.print("Parking: ");
    lcd.print(parkingSlots);
    lcd.print("/");
    lcd.print(maxParkingSlots);
    delay(1000); // Brief delay for message display
    lcd.clear();
  } else if (distance >= distanceThreshold) {
    carDetected = false; // Reset detection when no car is present
  }

  // Gate 1 monitoring
  if (digitalRead(GATE1_BUTTON) == LOW) {
    gate1Attendees++;
    totalAttendees++;
    activateTicketConfirmation();
    displayGateInfo();
  }

  // Gate 2 monitoring
  if (digitalRead(GATE2_BUTTON) == LOW) {
    gate2Attendees++;
    totalAttendees++;
    activateTicketConfirmation();
    displayGateInfo();
  }

  // Display real-time parking info and total attendees
  lcd.setCursor(0, 0);
  lcd.print("Cars: ");
  lcd.print(parkingSlots);
  lcd.print("/");
  lcd.print(maxParkingSlots);

  lcd.setCursor(0, 1);
  lcd.print("G1:");
  lcd.print(gate1Attendees);
  lcd.print(" G2:");
  lcd.print(gate2Attendees);

  delay(500); // Small delay for stable display refresh
}

// Simulates ticket confirmation by activating the LED for 1 second
void activateTicketConfirmation() {
  digitalWrite(LED_PIN, HIGH); // Turn on LED for ticket confirmation
  delay(1000);
  digitalWrite(LED_PIN, LOW);  // Turn off LED after confirmation
}

// Displays the gate attendee information on the LCD
void displayGateInfo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gate1: ");
  Serial.print("gate 1: ");
  Serial.print(gate1Attendees);
   Serial.print("gate 2: ");
  Serial.print(gate2Attendees);
  lcd.print(gate1Attendees);
  lcd.setCursor(0, 1);
  lcd.print("Gate2: ");
  lcd.print(gate2Attendees);

  delay(1000); // Display for 1 second

  // Show total attendees after gate info
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Total Attendees:");
  lcd.setCursor(0, 1);
  lcd.print(totalAttendees);

  Serial.print(totalAttendees);
  delay(1000); // Display for 1 second
}
