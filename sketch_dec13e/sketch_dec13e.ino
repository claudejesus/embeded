#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2); // I2C address 0x20, 16x2 LCD
Servo myservo;

// Pins for ultrasonic sensor and LEDs
#define trigPin 6
#define echoPin 7
#define redLedPin 12
#define greenLedPin 13
#define ground 11

// Variable resistor and manual override
#define variableResistorPin A0
#define ExitButtonPin 2

// Parking system variables
const int totalSpaces = 5;
int freeSpaces = totalSpaces;
int parkedCars = 0;

// Ultrasonic sensor variables
long duration;

// Initialize the system
void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud rate
  Serial.println("kjhiguyfdzxghh");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(ground, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(ExitButtonPin, INPUT_PULLUP);

  digitalWrite(ground, LOW);
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, HIGH);

  myservo.attach(10);
  lcd.begin(16, 2);

  Serial.println("Parking System Initialized"); // Debug message
  lcd.print("Parking System");
  delay(2000);
  lcd.clear();
  updateLCD();
}

void loop() {
  int distance = measureDistance(); // Measure distance from ultrasonic sensor
  int manualOverride = analogRead(variableResistorPin); // Read variable resistor value

  // Debugging serial prints for distance and manual override
  Serial.print("Measured Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.print("Manual Override Value: ");
  Serial.println(manualOverride);

  // Handle car entry
  if (freeSpaces > 0 && distance > 0 && distance < 50) {
    Serial.println("Car detected! Attempting entry...");
    handleCarEntry();
  }

  // Handle car exit
  if (digitalRead(ExitButtonPin) == LOW && freeSpaces < totalSpaces) {
    Serial.println("Exit button pressed! Attempting exit...");
    handleCarExit();
  }

  // Update LEDs and display
  updateLEDs();
  updateLCD();

  delay(100); // Delay for smoother operation
}

// Handle car entry
void handleCarEntry() {
  freeSpaces--;
  parkedCars++;

  // Serial debug prints
  Serial.println("Car Entered:");
  Serial.print("Free Spaces: ");
  Serial.println(freeSpaces);
  Serial.print("Parked Cars: ");
  Serial.println(parkedCars);

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

  // Serial debug prints
  Serial.println("Car Exited:");
  Serial.print("Free Spaces: ");
  Serial.println(freeSpaces);
  Serial.print("Parked Cars: ");
  Serial.println(parkedCars);

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
  int distance = duration * 0.034 / 2; // Convert to cm

  // Serial debug print for distance
  Serial.print("Ultrasonic Sensor Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

// Open gate
void openGate() {
  Serial.println("Opening Gate...");
  for (int pos = 0; pos <= 90; pos++) {
    myservo.write(pos);
    delay(15);
  }
  Serial.println("Gate Opened.");
}

// Close gate
void closeGate() {
  Serial.println("Closing Gate...");
  for (int pos = 90; pos >= 0; pos--) {
    myservo.write(pos);
    delay(15);
  }
  Serial.println("Gate Closed.");
}

// Update LEDs based on parking availability
void updateLEDs() {
  if (freeSpaces == 0) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    Serial.println("Parking Full. Red LED ON.");
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    Serial.println("Spaces Available. Green LED ON.");
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

  // Serial debug prints
  Serial.print("LCD Display -> Free: ");
  Serial.print(freeSpaces);
  Serial.print("/");
  Serial.println(totalSpaces);
  Serial.print("Parked Cars: ");
  Serial.println(parkedCars);
}