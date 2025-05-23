#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h  >

LiquidCrystal_I2C lcd(0x20, 16, 2);

// Define Pinsz

#define EXIT_BUTTON 9
#define GREEN_LED 3
#define RED_LED 2
#define TRIG_PIN 6
#define ECHO_PIN 5

Servo servo1;
int pos=0;
int potPin = A0;
int potValue =0;

int totalSpaces = 10; // Total parking spaces
int freeSpaces = 10;  // Initially, all spaces are free

bool objectInRange = false;      // Tracks if an object is in range
bool lastExitButtonState = HIGH; // Previous state of the exit button

void setup() {
  Serial.begin(9600);

  // Set up pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(EXIT_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  // pinMode(servoPin, OUTPUT);
  servo1.attach(10);

  // Initial LED states
  digitalWrite(GREEN_LED, HIGH); // Green LED ON
  digitalWrite(RED_LED, LOW);    // Red LED OFF


  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Display welcome message
  lcd.setCursor(0, 0);
  lcd.print("Welcome to Goico");
  lcd.setCursor(0, 1);
  lcd.print("Parking System");
  
  Serial.write("Welcome to Goico");
  Serial.write("Parking System\n");
  //delay(2000);

  // Display the initial number of free spaces
  updateDisplay();
}

void loop() {

  potValue = analogRead(potPin); // Read potentiometer value (0-1023)
  // Serial.print("Potentiometer Value: ");
  // Serial.print(potValue);  // Print raw potentiometer value

  long duration, distance;
  // Trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2000);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(2000);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin duration
  duration = pulseIn(ECHO_PIN, HIGH);
 
  // Calculate distance in cm
  distance = duration * 0.034 / 2;

  // Entry Logic: Check for object entering and leaving the range
  if (distance > 0 && distance < 60) { // Object detected within range
    if(distance <30) {
      if (!objectInRange) { // If this is a new object detection
      objectInRange = true; // Mark object as in range
      if (freeSpaces > 0) {
        freeSpaces--;
        updateStatus(); // Update LEDs and display
        Serial.println("Car Entered");
      
        delay(2000);
      }
    }
    }
    Serial.print(distance);
    Serial.println(" cm");
    Serial.print("Potentiometer Value: ");
    Serial.println(potValue);
  } else {
    objectInRange = false; // Reset when object leaves range
  }

  // Exit Logic
  bool currentExitButtonState = digitalRead(EXIT_BUTTON);
  if (currentExitButtonState == LOW && lastExitButtonState == HIGH) {
    //delay(2000); // Debounce
    if (freeSpaces < totalSpaces) {
      freeSpaces++;
      updateStatus(); // Update LEDs and display
      Serial.println("Car Exited");
      Serial.println("");
    }
         for (pos = 90; pos >= 0; pos -= 1)              // goes from 180 degrees to 0 degrees                                                                                 // in steps of 1 degree
    {
      servo1.write (pos);                                                  // tell servo to go to position in variable 'pos'
      // / waits 10ms for the servo to reach the position
    }
  }
  //resistance variable
  potValue = analogRead(potPin);
  if (potValue > 512) {
    // openGate();

    for(pos = -90; pos <= 0; pos += 1) 
  {
    servo1.write(pos);              
    // delay(15);                      
  }  
  }

  // Update the last state of the exit button
  lastExitButtonState = currentExitButtonState;
}

// Function to update LEDs based on free spaces
void updateLEDs() {
  if (freeSpaces == 0) {

    digitalWrite(GREEN_LED, LOW);  // No space available
    digitalWrite(RED_LED, HIGH); 
     for (pos = 0; pos <= 90; pos += 1)              // goes from 180 degrees to 0 degrees                                                                                 // in steps of 1 degree
    {
      servo1.write (pos);                                                  // tell servo to go to position in variable 'pos'
    
    } // Red LED ON
  } else {
    digitalWrite(GREEN_LED, HIGH); // Space available
    digitalWrite(RED_LED, LOW);   // Red LED OFF
  }
}

// Function to update the LCD display
void updateDisplay() {
  Serial.print("Spaces Available: ");
  Serial.print(freeSpaces);
  Serial.print(" / ");
  Serial.println(totalSpaces);

}

// Function to update both LEDs and LCD
void updateStatus() {
  updateLEDs();    // Update LEDs based on free spaces
  updateDisplay(); // Update LCD with the current status
}