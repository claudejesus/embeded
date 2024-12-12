// Define the pin where the LED is connected
#define LED_PIN 2  // GPIO pin 2 is commonly used for onboard LEDs

void setup() {
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Turn the LED on
  digitalWrite(LED_PIN, HIGH);
  delay(1000); // Wait for 1 second

  // Turn the LED off
  digitalWrite(LED_PIN, LOW);
  delay(1000); // Wait for 1 second
}
