const int LED = 13;
const int BUTTON = 3;
int val = 0;
int old_val = 0;
int state = 0;

void setup() {
  pinMode(LED,OUTPUT);
  pinMode(BUTTON,INPUT);
}

void loop() {
  val = digitalRead(BUTTON);
 
 
    digitalWrite(LED,HIGH);
    delay(2000);
    digitalWrite(LED,LOW);
    delay(3000);
    digitalWrite(LED,HIGH);
    delay(4000);
    digitalWrite(LED,LOW);
    delay(5000);
}
