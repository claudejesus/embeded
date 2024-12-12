// Pin where the LED is connected
const int ledPin = 13;       // LED1 (e.g., Red)
const int ledPinGreen = 12;  // LED2 (e.g., Green)
const int groundPin = 11;    // Ground pin for LEDs

void setup() {
  // Initialize the LED pins as outputs
  pinMode(ledPin, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);

  // Set the ground pin as output and set it to LOW
  pinMode(groundPin, OUTPUT);
  digitalWrite(groundPin, LOW);
}

void loop() {
  // Turn both LEDs ON for 2 seconds
  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPinGreen, HIGH);
  delay(2000); // Wait for 2 seconds

  // Turn both LEDs OFF for 3 seconds
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPinGreen, LOW);
  delay(3000); // Wait for 3 seconds

  // Turn LED1 ON and LED2 OFF for 4 seconds
  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPinGreen, LOW);
  delay(4000); // Wait for 4 seconds

  // Turn both LEDs OFF for 5 seconds
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPinGreen, LOW);
  delay(5000); // Wait for 5 seconds

  // Loop will repeat
}
