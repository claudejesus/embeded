
int ledbiby= 10;
int ledblue =6;
int button1 =11;
int button2 =5;
int buttonstatus1 = 0;
int buttonstatus2 = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledbiby, OUTPUT);
  pinMode(ledblue, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  digitalWrite(ledbiby, LOW);
  digitalWrite(ledblue, LOW);
  digitalWrite(button1, HIGH);
  digitalWrite(button2, HIGH);


  buttonstatus1 = digitalRead(button1);
  buttonstatus2 = digitalRead(button2);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(buttonstatus1 == LOW){
  digitalWrite(ledbiby, HIGH);
  }
  else {
  digitalWrite(ledbiby, LOW);
  }

  if(buttonstatus2 == LOW){
      digitalWrite(ledblue, HIGH);
  }
 else {
  digitalWrite(ledblue, LOW);
  }
}
