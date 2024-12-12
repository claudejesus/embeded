// Define pins for push buttons and LEDs
#define ENTRY_BUTTON 2
#define EXIT_BUTTON 3
#define GREEN_LED 4
#define RED_LED 5

// Define pins for 7-segment display
#define SEG_A 6
#define SEG_B 7
#define SEG_C 8
#define SEG_D 9
#define SEG_E 10
#define SEG_F 11
#define SEG_G 12

// Variables
int totalSpaces = 9; // Total parking spaces (0-9)
int freeSpaces = 9;  // Initially all spaces are free

// 7-segment display digit representation
const int digit[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
};

void setup() {
  // Set up button pins
  pinMode(ENTRY_BUTTON, INPUT_PULLUP);
  pinMode(EXIT_BUTTON, INPUT_PULLUP);

  // Set up LED pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH); // Green LED ON initially
  digitalWrite(RED_LED, LOW);   // Red LED OFF initially

  // Set up 7-segment display pins
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);

  // Display initial free spaces
  updateDisplay(freeSpaces);
}

void loop() {
  // Entry Button Logic
  if (digitalRead(ENTRY_BUTTON) == LOW) {
    delay(200); // Debouncing
    if (freeSpaces > 0) {
      freeSpaces--;
    }
    updateLEDs();
    updateDisplay(freeSpaces);
  }

  // Exit Button Logic
  if (digitalRead(EXIT_BUTTON) == LOW) {
    delay(200); // Debouncing
    if (freeSpaces < totalSpaces) {
      freeSpaces++;
    }
    updateLEDs();
    updateDisplay(freeSpaces);
  }
}

// Function to update LEDs based on free space
void updateLEDs() {
  if (freeSpaces == 0) {
    digitalWrite(GREEN_LED, LOW); // No space available
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, HIGH); // Space available
    digitalWrite(RED_LED, LOW);
  }
}

// Function to update the 7-segment display
void updateDisplay(int value) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(SEG_A + i, digit[value][i]);
  }
}
