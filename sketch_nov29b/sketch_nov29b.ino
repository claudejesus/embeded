const int LED = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT); 
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(LED,HIGH);
delay(2000);
digitalWrite(LED,LOW);
delay(3000);
digitalWrite(LED,HIGH);
delay(4000);
digitalWrite(LED,LOW);
delay(5000);
}
