//* Libraries *//
#include <IRremote.h>
//* Variables *//
//IR Configuration
const uint8_t recvPin = 11;
IRrecv irrecv(recvPin);
decode_results results;
uint8_t button = NULL;
//Serial
String serialBuffer = "";
char serialReading = '\0';
//Sensors
int sensorLastReading = 0;
int sensorReading = 0;
unsigned long sensorNow = 0;
unsigned long sensorLastTime = 0;
uint8_t checkThisSensor = 16;
uint8_t sensorCurrent = 0;
//Lights
uint8_t lightFirst = 2;
uint8_t lightLast = 9;
uint8_t lightCurrent = 0;
//Blink
unsigned long blinkOffTime = 0;
//Routines
uint8_t currentRoutine = 0;

void setup() {
  Serial.begin(115200);
  Serial.print("\rIntelliLight v0.0.1\r\n");
  Serial.print("ilc> ");
}

void loop() {
  doRoutine ();
  logSensor();
  serialPrompt();
}

void serialPrompt() {
  if (Serial.available()) {
    serialReading = Serial.read();
    if (serialReading == '\r') {
      parseCommand(serialBuffer);
      Serial.print("\r\nilc> ");
      serialBuffer = "";
    } else {
      serialBuffer += serialReading;
      Serial.print(serialReading);
    }
  }
}

void parseCommand(String input) {
  String command = input.substring(0, input.indexOf(" "));
  String argument = input.substring(input.indexOf(" ")+1, input.length());
  if (command.equals("help")) {
    Serial.print("\r\nCommands:\r\n\tNo commands yet.");
  } else if (command.equals("read")) {
    checkThisSensor = argument.toInt();
  } else if (command.equals("do")) {
    currentRoutine = argument.toInt();
    digitalWrite(lightCurrent, LOW);
    lightCurrent = 0;
  } else if (command.equals("next")) {
    randomLight();
  } else if (command.equals("")) {
    if (currentRoutine == 1) {
      randomLight();
    }
    checkThisSensor = 16;
  } else {
    Serial.print("\r\nERR: \'"+command+"\' command not found.\r\n\targument: "+argument);
  }
}

void logSensor() {
  if (currentRoutine != 0) {
    return;
  }
  if (checkThisSensor != 16) {
    sensorNow = millis();
    if (sensorNow >= sensorLastTime + 25) {
      sensorReading = analogRead(checkThisSensor);
      sensorLastTime = sensorNow;
      if (sensorReading < sensorLastReading-3) {
        Serial.print("\r\nread: ");
        Serial.print(sensorReading);
        Serial.print("\r\nlast: ");
        Serial.print(sensorLastReading);
        Serial.print("\r\nDONE\r\n");
        Serial.print("ilc> ");
        sensorLastReading = 0;
        checkThisSensor = 16;
      }
      sensorLastReading = sensorReading;
    }
    if (millis()%100 == 0) {
      Serial.print("\rSensor A");
      Serial.print(checkThisSensor);
      Serial.print(": ");
      Serial.print(sensorReading);
    }
  }
}

void doRoutine() {
  switch(currentRoutine) {
    case 1:
    routineOne();
    break;
  }
}

void routineOne() {
  if (lightCurrent == 0) {
    randomLight();
  }
  sensorNow = millis();
  if (sensorNow >= sensorLastTime + 15) {
    sensorReading = analogRead(sensorCurrent);
    sensorLastTime = sensorNow;
    if (sensorReading < sensorLastReading - 4) {
      randomLight();
    }
    sensorLastReading = sensorReading;
  }
  if (sensorNow%100 == 0) {
    Serial.print("\rSensor A");
    Serial.print(sensorCurrent);
    Serial.print(": ");
    Serial.print(sensorReading);
  }
}

void randomLight() {
  digitalWrite(lightCurrent, LOW);
  lightCurrent = random(lightFirst, lightLast+1);
  switch(lightCurrent) {
    case 2:
    sensorCurrent = 4;
    break;
    case 3:
    sensorCurrent = 1;
    break;
    case 4:
    sensorCurrent = 3;
    break;
    case 5:
    sensorCurrent = 2;
    break;
    case 6:
    sensorCurrent = 7;
    break;
    case 7:
    sensorCurrent = 6;
    break;
    case 8:
    sensorCurrent = 5;
    break;
    case 9:
    sensorCurrent = 0;
    break;
  }
  Serial.print("\r\nSwitched light to ");
  Serial.print(lightCurrent);
  Serial.print("\r\n");
  digitalWrite(lightCurrent, HIGH);
  sensorLastReading = 0;
}
