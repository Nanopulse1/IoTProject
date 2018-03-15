/*

#include <Servo.h>
Servo myservo;
int pos = 0;

const int buzzerPin = 4;
int buzzerState = 0;
const int ledPin = 8;
int ledState = 0;
const int buttonPin = 6;
int buttonState = 0;

const int FUNCTION = 7;
int functionState = 0;

int LIGHT_SENSOR = A0;

const int LED = 3;

void setup() {
  // put your setup code here, to run once:
myservo.attach(2);

pinMode(LED, OUTPUT);
pinMode(FUNCTION, INPUT);
pinMode(buttonPin, INPUT);
pinMode(buzzerPin, OUTPUT);
pinMode(ledPin, OUTPUT);
functionState = 0;

Serial.begin(9600);

}

void loop() {
  int sensorValue = analogRead(LIGHT_SENSOR);
  buttonState = digitalRead(buttonPin);
  Serial.println(sensorValue);

  if(sensorValue <= 50){
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  }
  
  
  if(buttonState == digitalRead(HIGH)){
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

*/
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <Servo.h>

// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
BridgeServer server;
Servo myservo;

int pos = 0;
const int buttonPin = 6;
int buttonState = 0;

const int FUNCTION = 7;
int functionState = 0;

const int LED = 3;
const int PIR = 6;
int pirState = 0;

void setup() {
  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  myservo.attach(2);

pinMode(LED, OUTPUT);
digitalWrite(LED, LOW);
pinMode(PIR, INPUT);
pinMode(FUNCTION, OUTPUT);
pinMode(buttonPin, INPUT);
functionState = 0;

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

  functionState = digitalRead(FUNCTION);

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
    
    // Close connection and free resources.
    client.stop();
  }

  delay(50); // Poll every 50ms

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

