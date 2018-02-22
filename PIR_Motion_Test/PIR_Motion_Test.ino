#define PIR_MOTION_SENSOR 2
#define LED  4

void setup() {
  // put your setup code here, to run once:
  pinsInit();
}


void loop() {
  // put your main code here, to run repeatedly:
  if(motionDetected()== true)
  {
    digitalWrite(LED,HIGH);
    }
  else
  {
    digitalWrite(LED,LOW);
    }
}

void pinsInit()
{
  pinMode(PIR_MOTION_SENSOR, INPUT);
  pinMode(LED,OUTPUT);
}

boolean motionDetected()//Code adapted from GitHub repository for PIR_Motion sensor https://goo.gl/Roq8sE
{
  int sensorValue = digitalRead(PIR_MOTION_SENSOR);
  if(sensorValue == HIGH)
  {
    return true;
  }
  else
  {
    return false;
  }
}
