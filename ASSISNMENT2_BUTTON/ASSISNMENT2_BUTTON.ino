const int LED1 = 1;
const int LED = 2;

const int BUTTON = 3;
const int BUTTON2 = 0;
int val = 0;
int val2 = 0;
int old_val = 0;
int state = 0;

void setup() {
  pinMode(LED1,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(BUTTON,INPUT);
  pinMode(BUTTON2,INPUT);
}

void loop() {
  val = digitalRead(BUTTON);
  val2 = digitalRead(BUTTON2);
  
  if ((val == HIGH)&&(old_val == LOW) && (val2 == HIGH)) {
    state = 1 - state;
    delay(10);
  }
  if ((val == LOW)&&(old_val == HIGH)) {
    delay(10);
  }
  old_val = val;
  if (state == 1){
    digitalWrite(LED,HIGH);
  }else {
    digitalWrite(LED,LOW);
  }
}
