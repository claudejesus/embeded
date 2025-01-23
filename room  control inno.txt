#include <Wire.h>
#include "DHTesp.h"


DHTesp dht;                       // Create DHT object
int dhtPin = 2;                  // GPIO 15 for the DHT11 sensor
#define RELAY1_PIN 5              // Relay 1 pin (Heating) - GPIO 2
#define RELAY2_PIN 4              // Relay 2 pin (Cooling) - GPIO 4

void setup() {
         // Attach the I2C pins (SDA, SCL)
  dht.setup(dhtPin, DHTesp::DHT11); // Attach the DHT pin and initialize it

  pinMode(RELAY1_PIN, OUTPUT);    // Set relay 1 as output
  pinMode(RELAY2_PIN, OUTPUT);    // Set relay 2 as output

  // Initialize relays to OFF state
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);

  Serial.begin(115200);           // Initialize Serial Monitor with baud rate 115200
}

void loop() {
flag: TempAndHumidity DHT = dht.getTempAndHumidity();
  if (dht.getStatus() != 0) {       // Determine if the read is successful, and if it fails, go back to flag and re-read the data
    goto flag;
  }  

  // Print temperature and humidity to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(DHT.temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(DHT.humidity);
  Serial.println(" %");

  // Relay control logic
  if (DHT.temperature < 18) {       // If temperature is below 18°C
    digitalWrite(RELAY1_PIN, HIGH); // Turn ON Relay 1 (Heating)
    digitalWrite(RELAY2_PIN, LOW);  // Turn OFF Relay 2
    Serial.println("Heating ON");
  } else if (DHT.temperature > 40) { // If temperature is above 40°C
    digitalWrite(RELAY1_PIN, LOW);  // Turn OFF Relay 1
    digitalWrite(RELAY2_PIN, HIGH); // Turn ON Relay 2 (Cooling)
    Serial.println("Cooling ON");
  } else {
    // If temperature is within the range, turn off both relays
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    Serial.println("Temperature within range. Relays OFF.");
  }

  delay(2000); // Wait for 2 seconds before the next reading
}
