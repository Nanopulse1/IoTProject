#include <Servo.h>
Servo myservo;
int pos = 0;
const int buttonPin = 6;
int buttonState = 0; 

void setup() {
  // put your setup code here, to run once:
myservo.attach(2);
pinMode(buttonPin, INPUT);
}

void loop() {

  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    
    myservo.write(pos); 
  } 
  else if(buttonState == HIGH) {
    for (pos = 0; pos <= 180; pos += 1) {
    
    myservo.write(pos);              
    delay(15);
  
   
              }
  }
  else{
    pos=0;
  }
}

void pinsInit()
{
}


