#define PB1 0
#define PB2 1
int counter = 0;
//int PB1 = 0;
//int PB2 = 1;
const int LED1 = 6;
const int LED2 = 8;

void setup() {
  pinMode(PB1, INPUT_PULLUP);
  pinMode(PB2, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  Serial.begin(9600); // Optional: For debugging
}

void loop() {
  if (digitalRead(PB1) == LOW) { // PB1 pressed
    digitalWrite(LED1, HIGH);
    delay(1000); // LED1 ON for 1 second
    digitalWrite(LED1, LOW);
    counter++;
    Serial.println(counter); // Debugging output

    if (counter >= 10) {
      digitalWrite(LED2, HIGH); // LED2 ON if counter >= 10
    }
    while (digitalRead(PB1) == LOW); // Debounce
  }

  if (digitalRead(PB2) == LOW) { // PB2 pressed
    counter = 0; // Reset counter
    digitalWrite(LED2, LOW); // Turn off LED2
    Serial.println("Counter reset"); // Debugging output
    while (digitalRead(PB2) == LOW); // Debounce
  }
}
