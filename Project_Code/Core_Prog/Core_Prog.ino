#define PIR_MOTION_SENSOR 2
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void pinsInit()
{
  pinMode(PIR_MOTION_SENSOR, INPUT);
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
