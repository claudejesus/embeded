#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Pin definitions
#define TRIG_PIN 9
#define ECHO_PIN 8
#define LED_PIN 12
#define BUTTON1_PIN 13 // Car Exit
#define BUTTON2_PIN 10 // Reset Parking Slots

int distanceThreshold = 500;   // Threshold for car detection (in cm)
int parkingSlots = 0;         // Tracks number of cars in the parking
int maxParkingSlots = 3;     // Maximum parking slots available
bool carDetected = false;     // Tracks if a car is currently detected

void setup() {
  // Initialize serial and LCD
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("Parking System");
  delay(2000);
  lcd.clear();

  // Pin setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, LOW); // Turn off the LED initially
}

int getDistance() {
  delay(2000);
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
  // Measure distance
    // Regularly update the display
  int distance = getDistance();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  lcd.print("Slots: ");
  lcd.print(parkingSlots);
  lcd.print("/ ");
  lcd.print(maxParkingSlots);
  delay(500); // Refresh display every 0.5 seconds

  if (distance >= distanceThreshold) {
    carDetected = true;
  }

  // Detect car entry if distance is below the threshold
  if (distance < distanceThreshold && carDetected && parkingSlots < maxParkingSlots) {
    carDetected = false; // Prevent multiple entries for one car
    parkingSlots++;     // Increment parking slot count

    // Display updated information
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Car Entered");
    lcd.setCursor(0, 1);
    lcd.print("Slots: ");
    lcd.print(parkingSlots);
    lcd.print("/ ");
    lcd.print(maxParkingSlots);
    Serial.println("Car Entered");
    delay(1000); // Small delay to stabilize detection
  }

  // Reset car detection when no object is detected


  // Handle car exit using Button 1
  if (digitalRead(BUTTON1_PIN) == LOW) {
    if (parkingSlots > 0) {
      parkingSlots--; // Decrease parking slot count
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Car Exited");
    lcd.setCursor(0, 1);
    lcd.print("Slots: ");
    lcd.print(parkingSlots);
    lcd.print("/ ");
    lcd.print(maxParkingSlots);
    Serial.println("Car Exited");
    delay(500); // Debounce delay
  }

  // Handle reset using Button 2
  if (digitalRead(BUTTON2_PIN) == LOW) {
    parkingSlots = 0; // Reset parking slots
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Slots Reset");
    lcd.setCursor(0, 1);
    lcd.print("Slots: ");
    lcd.print(parkingSlots);
    lcd.print("/ ");
    lcd.print(maxParkingSlots);
    Serial.println("Parking Slots Reset");
    delay(500); // Debounce delay
  }

  // LED control: Turn on LED when parking is full
  if (parkingSlots >= maxParkingSlots) {
    digitalWrite(LED_PIN, HIGH); // Turn on LED
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off LED
  }


}