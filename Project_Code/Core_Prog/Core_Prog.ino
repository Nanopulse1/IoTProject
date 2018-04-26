//Libraries
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <Servo.h>

// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
BridgeServer server;
Servo myservo;
Servo TeaDisp;

//Buzzer - Box Alarm
const int buzzerPin = 4;
int buzzerState = 0;

//tea dispenser
const int DialPin = A3;
int DialState = 0;

//LED - Box Alarm for deaf/hard of hearing users 
const int ledPin = 8;
int ledState = 0;

//Initalises Servo Position
int pos = 0;
int sta = 0;

//Function PIN used to execute servo code from HTTP request
const int FUNCTION = 7;
int functionState = 0;

//Light Dependant Resitor used to detect if teabag/ biscuit box is left open.
int LIGHT_SENSOR = A0;

//Setup
void setup() {
  // Bridge startup
  Bridge.begin();

  // Servo Initalisation
  myservo.attach(2);
  TeaDisp.attach(3);

  //LED Init
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  //Function Port Int
  pinMode(FUNCTION, OUTPUT);
  functionState = 0;

  pinMode(DialPin, INPUT);
  DialState = 0;

  //Buzzer Init
  pinMode(buzzerPin, OUTPUT);

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)  
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Get clients coming from server
  BridgeClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

      FunctionServo();
      
    // Close connection and free resources.
    client.stop();
  }

  delay(50); // Poll every 50ms

  LEDSensor();

}

void process(BridgeClient client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "digital" command?
  if (command == "digital") {
    digitalCommand(client);
  }

  // is "analog" command?
  if (command == "analog") {
    analogCommand(client);
  }

  // is "mode" command?
  if (command == "mode") {
    modeCommand(client);
  }
}

void digitalCommand(BridgeClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  } else {
    value = digitalRead(pin);
  }

  // Send feedback to client
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);

  // Update datastore key with the current pin value
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

void analogCommand(BridgeClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/analog/5/120"
  if (client.read() == '/') {
    // Read value and execute command
    value = client.parseInt();
    analogWrite(pin, value);

    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  } else {
    // Read analog pin
    value = analogRead(pin);

    // Send feedback to client
    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}

void modeCommand(BridgeClient client) {
  int pin;

  // Read pin number
  pin = client.parseInt();

  // If the next character is not a '/' we have a malformed URL
  if (client.read() != '/') {
    client.println(F("error"));
    return;
  }

  String mode = client.readStringUntil('\r');

  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }

  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT!"));
    return;
  }

  client.print(F("error: invalid mode "));
  client.print(mode);
}

void LEDSensor(){
   int sensorValue = analogRead(LIGHT_SENSOR);
   
      Serial.println(sensorValue);

      //LDR Sensor Statement
      if(sensorValue >= 50){
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(ledPin, HIGH);
                           }
      else{
        digitalWrite(buzzerPin, LOW);
        digitalWrite(ledPin, LOW);
          } 
}

void FunctionServo(){
  functionState = digitalRead(FUNCTION);

      //Servo Statement
      if(functionState == digitalRead(HIGH)){
        for (pos = 0; pos <= 180; pos += 1) 
        {
        myservo.write(pos);              
        delay(15);
        }
        }
        else{
              pos=0;
            }
        functionState = LOW;
}
void DispenseServo(){
DialState = Serial.Read(DialPin);
        //Servo Statement
      if(DialState <= 2400){
          sta = 90;
          TeaDisp.write(sta);
          delay(15);
      }
      else if(DialState > 2400 && DialState < 4800)
      {
        sta = 180;
          TeaDisp.write(sta);
          delay(15);
          
      }
      else if(DialState >= 4800 && DialState < 7200)
      {
        sta = 270;
          TeaDisp.write(sta);
          delay(15);
      }
      else
      {
        sta = 0;
        TeaDisp.write(sta);
        delay(15);
      }
}

