int potPin = A0;  // Analog pin connected to the potentiometer
int ledPin = 3;   // PWM pin connected to the LED
int potValue = 0; // Variable to store the potentiometer value
int ledBrightness = 0; // Variable for mapped LED brightness

void setup() {
  pinMode(ledPin, OUTPUT); // Set the LED pin as output
  Serial.begin(9600);      // Initialize serial communication for debugging
}

void loop() {
  potValue = analogRead(potPin); // Read potentiometer value (0-1023)
  ledBrightness = map(potValue, 0, 1023, 0, 255); // Map to LED brightness (0-255)

  analogWrite(ledPin, ledBrightness); // Set LED brightness
  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);  // Print raw potentiometer value
  Serial.print(" -> LED Brightness: ");
  Serial.println(ledBrightness); // Print mapped brightness value
  
  delay(10); // Small delay for stability
}
