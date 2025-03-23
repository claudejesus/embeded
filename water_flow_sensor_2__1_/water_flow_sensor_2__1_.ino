#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Flow Sensors and Valves
#define FLOW_SENSOR_1 2
#define FLOW_SENSOR_2 3
#define FLOW_SENSOR_3 4
#define VALVE_1 7
#define VALVE_2 8
#define VALVE_3 9

// LEDs
#define RED_LED 5
#define GREEN_LED 6
#define RED_LED2 A3
#define GREEN_LED2 A2
#define RED_LED3 A0
#define GREEN_LED3 A1

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

byte rowPins[ROWS] = {10, 9, 8, 7};
byte colPins[COLS] = {11, 12, 13};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

volatile int pulseCount1 = 0, pulseCount2 = 0, pulseCount3 = 0;
float flowRate1 = 0.0, flowRate2 = 0.0, flowRate3 = 0.0, balance = 0.0;
unsigned long lastFlowTime = 0;
bool isLoggedIn = false;
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

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED2, OUTPUT);
  pinMode(GREEN_LED2, OUTPUT);
  pinMode(RED_LED3, OUTPUT);
  pinMode(GREEN_LED3, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_1), flowSensorISR1, RISING);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_2), flowSensorISR2, RISING);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_3), flowSensorISR3, RISING);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Login: Enter ID");

  Serial.begin(9600);
}

void loop() {
  static String inputID = "";
  
  if (!isLoggedIn) {
    char key = keypad.getKey();
    if (key) {
      Serial.print("Key Pressed: ");
      Serial.println(key);
      if (key == '#') {
        userID = inputID.toInt();
        if (userID >= 1 && userID <= 3) {
          isLoggedIn = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter Balance:");
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Invalid ID!");
          delay(2000);
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

  static String inputBalance = "";
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);
    if (key == '#') {
      balance = inputBalance.toFloat();
      inputBalance = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Balance: ");
      lcd.print(balance, 2);
    } else if (key >= '0' && key <= '9' || key == '.') {
      inputBalance += key;
    }
  }

  if (millis() - lastFlowTime > 1000) {
    flowRate1 = (pulseCount1 / 7.5);
    flowRate2 = (pulseCount2 / 7.5);
    flowRate3 = (pulseCount3 / 7.5);

    float totalUsage = (flowRate1 + flowRate2 + flowRate3) * 0.01;
    balance -= totalUsage;

    pulseCount1 = 0;
    pulseCount2 = 0;
    pulseCount3 = 0;
    lastFlowTime = millis();
    
    lcd.setCursor(0, 1);
    lcd.print("Bal: ");
    lcd.print(balance, 2);

    if (balance <= 0) {
      digitalWrite(VALVE_1, LOW);
      digitalWrite(VALVE_2, LOW);
      digitalWrite(VALVE_3, LOW);
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED2, HIGH);
      digitalWrite(GREEN_LED2, LOW);
      digitalWrite(RED_LED3, HIGH);
      digitalWrite(GREEN_LED3, LOW);
      lcd.setCursor(10, 1);
      lcd.print("CLOSED ");
    } else {
      digitalWrite(VALVE_1, HIGH);
      digitalWrite(VALVE_2, HIGH);
      digitalWrite(VALVE_3, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED2, LOW);
      digitalWrite(GREEN_LED2, HIGH);
      digitalWrite(RED_LED3, LOW);
      digitalWrite(GREEN_LED3, HIGH);
      lcd.setCursor(10, 1);
      lcd.print("OPEN   ");
    }
  }
}