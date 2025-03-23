#include <WiFi.h>
#include <HTTPClient.h>
#include <Servo.h>

// WiFi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Google Sheets API endpoint
const char* serverName = "https://script.google.com/macros/s/YOUR_SCRIPT_ID/exec";

// Define components
const int entryButton = 5;
const int exitButton = 18;
const int manualUpdateButton = 19;
const int parkingSpots = 10; // Initial total parking spots
Servo gateServo;

int availableSpots = parkingSpots;

void setup() {
    Serial.begin(115200);
    pinMode(entryButton, INPUT_PULLUP);
    pinMode(exitButton, INPUT_PULLUP);
    pinMode(manualUpdateButton, INPUT_PULLUP);
    gateServo.attach(13);
    gateServo.write(0); // Gate closed position
    connectWiFi();
}

void loop() {
    if (digitalRead(entryButton) == LOW) {
        vehicleEntry();
    }
    if (digitalRead(exitButton) == LOW) {
        vehicleExit();
    }
    if (digitalRead(manualUpdateButton) == LOW) {
        manualUpdate();
    }
}

void connectWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected!");
}

void vehicleEntry() {
    if (availableSpots > 0) {
        availableSpots--;
        Serial.println("Vehicle Entered. Updating cloud...");
        openGate();
        updateCloud();
    } else {
        Serial.println("Parking Full!");
    }
}

void vehicleExit() {
    availableSpots++;
    Serial.println("Vehicle Exited. Updating cloud...");
    openGate();
    updateCloud();
}

void manualUpdate() {
    Serial.println("Manual update triggered!");
    updateCloud();
}

void openGate() {
    gateServo.write(90); // Open gate
    delay(3000);
    gateServo.write(0); // Close gate
}

void updateCloud() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(serverName) + "?spots=" + availableSpots;
        http.begin(url);
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            Serial.println("Data Sent Successfully!");
        } else {
            Serial.print("Error: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    } else {
        Serial.println("WiFi not connected!");
    }
}
