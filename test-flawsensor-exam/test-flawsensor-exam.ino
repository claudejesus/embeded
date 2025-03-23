#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Flow Sensors and Valves
#define FLOW_SENSOR_1 2  // Attach to interrupt pin
#define FLOW_SENSOR_2 3  // Attach to interrupt pin
#define FLOW_SENSOR_3 4  // Moved to an input pin, but check your board's compatibility
#define VALVE_1 A5
#define VALVE_2 A4
#define VALVE_3 A3

// LEDs
#define RED_LED1 5
#define GREEN_LED1 6
#define RED_LED2 A2
#define GREEN_LED2 A1
#define RED_LED3 A0
#define GREEN_LED3 9  // Changed to avoid conflict with Keypad

// LCD Configuration
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Keypad Configuration
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {10, 11, 12, 13};  // Adjusted to avoid conflict
byte colPins[COLS] = {A7, A6, A5};  // Adjusted to available pins

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

volatile int pulseCount1 = 0, pulseCount2 = 0, pulseCount3 = 0;
float flowRate1 = 0.0, flowRate2 = 0.0, flowRate3 = 0.0;
float houseBalances[3] = {10.0, 10.0, 10.0};  // Starting balances
unsigned long lastFlowTime = 0;
bool isLoggedIn = false, topUpMode = false;
int userID = -1;

// Interrupt Service Routines
void flowSensorISR1() { pulseCount1++; }
void flowSensorISR2() { pulseCount2++; }
void flowSensorISR3() { pulseCount3++; }

void setup() {
  pinMode(FLOW_SENSOR_1, INPUT);
  pinMode(FLOW_SENSOR_2, INPUT);
  pinMode(FLOW_SENSOR_3, INPUT);
  pinMode(VALVE_1, OUTPUT);
  pinMode(VALVE_2, OUTPUT);
  pinMode(VALVE_3, OUTPUT);

  pinMode(RED_LED1, OUTPUT);
  pinMode(GREEN_LED1, OUTPUT);
  pinMode(RED_LED2, OUTPUT);
  pinMode(GREEN_LED2, OUTPUT);
  pinMode(RED_LED3, OUTPUT);
  pinMode(GREEN_LED3, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_1), flowSensorISR1, RISING);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_2), flowSensorISR2, RISING);
  // No interrupt on FLOW_SENSOR_3, use polling for it

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Login: Enter ID");

  Serial.begin(9600);
}

void loop() {
  static String inputID = "";
  static String topUpAmount = "";

  if (!isLoggedIn) {
    char key = keypad.getKey();
    if (key) {
      if (key == '#') {
        userID = inputID.toInt();
        if (userID >= 1 && userID <= 3) {
          isLoggedIn = true;
          
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Login Successful!");
          delay(1000);
          
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("House ");
          lcd.print(userID);
          lcd.setCursor(0, 1);
          lcd.print("Bal: ");
          lcd.print(houseBalances[userID - 1], 2);
          delay(1000);

          updateLEDs();
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Invalid ID!");
          delay(1000);
          lcd.clear();
          lcd.print("Login: Enter ID");
        }
        inputID = "";
      } else if (key >= '0' && key <= '9') {
        inputID += key;
      }
    }
    return;
  }

  if (topUpMode) {
    char key = keypad.getKey();
    if (key) {
      if (key == '#') {
        houseBalances[userID - 1] += topUpAmount.toFloat();
        topUpAmount = "";
        topUpMode = false;
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("House ");
        lcd.print(userID);
        lcd.setCursor(0, 1);
        lcd.print("Bal: ");
        lcd.print(houseBalances[userID - 1], 2);
        delay(1000);
        updateLEDs();
      } else if (key >= '0' && key <= '9' || key == '.') {
        topUpAmount += key;
      }
    }
    return;
  }

  char key = keypad.getKey();
  if (key) {
    if (key == '*') {
      topUpMode = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Top-up Amt:");
      return;
    }
    
    if (key == '0') { // Logout feature
      isLoggedIn = false;
      userID = -1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Logged Out!");
      delay(1000);
      lcd.clear();
      lcd.print("Login: Enter ID");
      return;
    }
  }

  if (millis() - lastFlowTime > 1000) {
    flowRate1 = (pulseCount1 / 7.5);
    flowRate2 = (pulseCount2 / 7.5);
    flowRate3 = (pulseCount3 / 7.5);

    float totalUsage = (flowRate1 + flowRate2 + flowRate3) * 0.01;
    
    if (houseBalances[userID - 1] > totalUsage) {
      houseBalances[userID - 1] -= totalUsage;
    } else {
      houseBalances[userID - 1] = 0; 
    }

    pulseCount1 = 0;
    pulseCount2 = 0;
    pulseCount3 = 0;
    lastFlowTime = millis();
    
    lcd.setCursor(0, 1);
    lcd.print("Bal: ");
    lcd.print(houseBalances[userID - 1], 2);

    updateLEDs();
  }
}

void updateLEDs() {
  digitalWrite(VALVE_1, userID == 1 && houseBalances[0] > 0);
  digitalWrite(VALVE_2, userID == 2 && houseBalances[1] > 0);
  digitalWrite(VALVE_3, userID == 3 && houseBalances[2] > 0);
}
