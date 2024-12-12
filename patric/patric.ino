#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2); // I2C address 0x20, 16x2 LCD

// Pins for ultrasonic sensor
#define trigPin  6
#define echoPin  5

// Pins for LEDs
#define redLedPin  2
#define greenLedPin 3 // New green LED pin

// Servo motor
//Servo gateServo;
Servo myservo;  
int pos = 7;    
// Variables
const int totalSpaces = 5; // Total parking spaces
int freeSpaces = 5; // Available parking spaces
int parkedCars = 0; // Number of parked cars

// Variable resistor and manual override
#define variableResistorPin A0 // Analog pin for variable resistor
int manualGateOverride = 0;

// Button for simulating car entry
#define carEntryButtonPin 8
bool carDetected = false;

// Ultrasonic sensor variables
long duration;
int carHeight;

// Servo motor positions
const int gateOpen = 90;   // Open position
const int gateClosed = 0;  // Closed position

void setup() {
  // Initialize pins
  Serial.begin(9600);
  Serial.write("sdfghjk");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT); // Initialize green LED pin
  pinMode(carEntryButtonPin, INPUT_PULLUP);
  pinMode(variableResistorPin, INPUT);

  // digital.Read(variableResistorPin);
  // Initialize servo and LCD
  //gateServo.attach(5); // Servo connected to pin 5
  myservo.attach(5);
  //gateServo.write(gateClosed); // Start with gate closed
  lcd.begin(16, 2);

  // Display initial message
  
  lcd.print("Parking System");
  delay(2000);
  lcd.clear();
  lcd.println(manualGateOverride);
  delay(5000);
  lcd.clear();
  updateLCD();
}

void loop() {
  // Check for car entry
  carDetected = digitalRead(carEntryButtonPin) == LOW;
  int distance = measureCarHeight();
  if(freeSpaces > 0){
  if (distance > 0 && distance <50) {
    handleCarEntry();
  }
  }

  

  // Update LEDs
  updateLEDs();

  // Display information
  updateLCD();

  delay(100);

  //closeGate();
}

void handleCarEntry() {
  // Measure car height using ultrasonic sensor
  carHeight = measureCarHeight();
  int metero = measureCarHeight();

  manualGateOverride = analogRead(variableResistorPin);
  // Update parking space data
  freeSpaces--;
  parkedCars++;

  // Display car height on LCD
  lcd.clear();
  lcd.print("Car Height: ");
  lcd.print(metero);
  lcd.print(" cm");
  lcd.setCursor(0, 1);
  lcd.print(manualGateOverride);
  delay(2000);

  // Open the gate
  openGate();
  delay(3000);

  // Close the gate automatically
  //closeGate();
}

int measureCarHeight() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}

void openGate() {
  //gateServo.write(gateOpen);
}

void closeGate() {
  //gateServo.write(gateClosed);
  //Check for manual gate override
  manualGateOverride = analogRead(variableResistorPin);
  if (manualGateOverride > 512) {
    // openGate();

    for(pos = -90; pos <= 0; pos += 1) 
  {
    myservo.write(pos);              
    // delay(15);                      
  }  
  }
}

void updateLEDs() {
  if (freeSpaces == 0) {
    digitalWrite(redLedPin, HIGH);   // Turn on Red LED
    digitalWrite(greenLedPin, LOW); // Turn off Green LED

    for(pos = 90; pos <= 180; pos += 1) 
  {
    myservo.write(pos);              
    // delay(15);                      
  } 
  // for(pos = 180; pos>=0; pos-=1)     
  // {                                
  //   myservo.write(pos);             
  //   delay(15);                      
  // } 
  closeGate();
  } else {
    digitalWrite(redLedPin, LOW);   // Turn off Red LED
    digitalWrite(greenLedPin, HIGH); // Turn on Green LED
  }



}


void updateLCD() {
  lcd.clear();
  lcd.print("Free: ");
  lcd.print(freeSpaces);
  Serial.write(freeSpaces);
  lcd.print("/");
  lcd.print(totalSpaces);

  lcd.setCursor(0, 1);
  lcd.print("Parked: ");
  lcd.print(parkedCars);
}