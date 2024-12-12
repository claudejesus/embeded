#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C Address may vary

const int PB1 = 8;  // Entry Button
const int PB2 = 9;  // Exit Button
const int redLED = 10;

int totalSpaces = 10;  // Total parking spaces
int freeSpaces = 10;   // Initially, all spaces are free
int parkedCars = 0;    // No cars parked initially

void setup() {
  pinMode(PB1, INPUT_PULLUP);
  pinMode(PB2, INPUT_PULLUP);
  pinMode(redLED, OUTPUT);
  
  lcd.begin();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Free:");
  lcd.setCursor(0, 1);
  lcd.print("Parked:");
  
  updateLCD();
}

void loop() {
  if (digitalRead(PB1) == LOW) {  // Car Entry
    if (freeSpaces > 0) {
      freeSpaces--;
      parkedCars++;
      updateLCD();
      if (freeSpaces == 0) {
        digitalWrite(redLED, HIGH); // Full Parking Lot
      }
    }
    delay(500); // Debounce
  }

  if (digitalRead(PB2) == LOW) {  // Car Exit
    if (parkedCars > 0) {
      freeSpaces++;
      parkedCars--;
      updateLCD();
      digitalWrite(redLED, LOW); // Free space available
    }
    delay(500); // Debounce
  }
}

void updateLCD() {
  lcd.setCursor(5, 0);
  lcd.print(freeSpaces);
  lcd.setCursor(8, 0);
  lcd.print("/");
  lcd.setCursor(9, 0);
  lcd.print(totalSpaces);

  lcd.setCursor(8, 1);
  lcd.print(parkedCars);
}
