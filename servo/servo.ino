#include <Servo.h> // Include the Servo library

Servo myServo; // Create a servo object

void setup() {
  myServo.attach(10); // Attach the servo to pin 9
  Serial.begin(9600); // Start serial communication for debugging (optional)
  Serial.write("eretyfguyhujk");
}

void loop() {
  // Sweep the servo from 0 to 180 degrees
  for (int pos = 0; pos <= 180; pos++) {
    myServo.write(pos); // Move to position
    delay(15);          // Wait 15 ms for the servo to reach the position
  }
  
  // Sweep back from 180 to 0 degrees
  for (int pos = 180; pos >= 0; pos--) {
    myServo.write(pos); // Move to position
    delay(15);          // Wait 15 ms for the servo to reach the position
  }
}
