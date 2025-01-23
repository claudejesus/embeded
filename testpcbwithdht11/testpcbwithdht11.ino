#include <DHT.h>
#include <Adafruit_Sensor.h>  // Required for the DHT library to work

// Define pin connections
#define DHTPIN A0      // Pin connected to the DHT11 sensor
#define DHTTYPE DHT11  // Define the sensor type (DHT11)

#define RELAY1 7  // Pin for Relay 1
#define RELAY2 8  // Pin for Relay 2

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize relays as output
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  // Start serial communication for debugging
  Serial.begin(9600);

  // Initialize the DHT sensor
  dht.begin();

  // Turn off both relays initially
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
}

void loop() {
  // Read temperature from the DHT11 sensor
  float temperature = dht.readTemperature();

  // Check if the reading is valid
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Debugging: print the temperature value
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  // Check temperature and control relays
  if (temperature > 40) {
    // Activate Relay 1 (Relay 2 OFF)
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, LOW);
    Serial.println("Relay 1 is ON (Temperature > 40°C)");
  } else if (temperature >= 30 && temperature <= 40) {
    // Activate Relay 2 (Relay 1 OFF)
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, HIGH);
    Serial.println("Relay 2 is ON (30°C <= Temperature <= 40°C)");
  } else {
    // Deactivate both relays
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
    Serial.println("Both relays are OFF (Temperature < 30°C)");
  }

  // Wait for a while before reading again
  delay(2000);
}
