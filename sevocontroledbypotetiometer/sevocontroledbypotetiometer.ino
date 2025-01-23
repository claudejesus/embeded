#include <Servo.h>   
 Servo servoa;  
// int servoa =9;

int potpin1 = 0;  
int val1;   



void setup() 
{ 
  servoa.attach(9);  
  pinMode(ledred1, OUTPUT);
  pinMode(ledblue, OUTPUT);
  digitalWrite(ledred1, LOW);
  digitalWrite(ledblue, LOW);
 
} 

void loop() 
{ 
  val1 = analogRead(potpin1);            
  val1 = map(val1, 0, 1023, 0, 179);      
  servoa.write(val1);                  
  delay(15);       

 
}