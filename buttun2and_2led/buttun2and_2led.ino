#define button_pin 4
#define button_pin2 5

const int LED1 = 6;
const int LED2 = 8;
int button_pressed_count = 0;
int button_state = 0;
int prev_button_state = 0;

void setup() {
  // put your setup code here, to run once:

pinMode(button_pin, INPUT_PULLUP);
pinMode(button_pin2, INPUT_PULLUP);
pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
digitalWrite(2, LOW);
digitalWrite(3, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

button_state = digitalRead(button_pin);

if(button_state != prev_button_state){
  // digitalWrite(2, HIGH);
  // delay(500);
  if(button_state == HIGH){
    digitalWrite(2, LOW);
    //delay(500);
  } else if(button_state == LOW){
    digitalWrite(2, HIGH);
    button_pressed_count++;
    if(button_pressed_count == 10){
      digitalWrite(3, HIGH);
      // delay(1000);
      // digitalWrite(3, LOW);
    }else if((button_state == LOW)&&(digitalRead(button_pin2) == HIGH)){
      button_pressed_count == 0;
      digitalWrite(3, LOW);
    }
  } 
  delay(1000);
}
prev_button_state = button_state;

}