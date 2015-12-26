//* Libraries *//
#include <IRremote.h>
//* Variables *//
//IR Configuration
const uint8_t recvPin = 11;
IRrecv irrecv(recvPin);
decode_results results;
uint8_t button = 0;
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
int readings[30];
uint8_t calib = 10;
//Lights
uint8_t lightFirst = 2;
uint8_t lightLast = 9;
uint8_t lightCurrent = 0;
uint8_t lightLastOn = 0;
//Routines
uint8_t currentRoutine = 0;
unsigned long routineEnd = 0;
unsigned long routineLightEnd = 0;
unsigned long routineLightEndEnd = 0;
bool routineLightOn = 0;
//Divider
double R1 = 807.0;
double R2 = 220.0;
double voltage = 0.0;
const uint8_t redLed = 52;
const uint8_t yellowLed = 53;
const uint8_t greenLed = 50;

void setup() {
  pinMode(52, OUTPUT);
  pinMode(53, OUTPUT);
  pinMode(50, OUTPUT);
  irrecv.enableIRIn();
  Serial.begin(115200);
  Serial.print("\rIntelliLight v0.0.1\r\n");
  Serial.print("ilc> ");
}

void loop() {
  checkIr();
  checkVoltage();
  doRoutine ();
  logSensor();
  serialPrompt();
}

void checkIr() {
  if (irrecv.decode(&results)) {
    switch(results.value) {
      case 0x20DF10EF://Power
      button = 10;
      currentRoutine = 0;
      clearLights();
      break;
      case 0x20DF8877://1
      button = 1;
      clearLights();
      currentRoutine = 1;
      sensorLastTime = millis();
      break;
      case 0x20DF48B7://2
      clearLights();
      currentRoutine = 2;
      button = 2;
      break;
      case 0x20DFC837://3
      clearLights();
      currentRoutine = 3;
      button = 3;
      break;
      case 0x20DF28D7://4
      clearLights();
      currentRoutine = 4;
      button = 4;
      break;
      case 0x20DFA857://5
      clearLights();
      currentRoutine = 5;
      button = 5;
      break;
      case 0x20DF6897://6
      clearLights();
      currentRoutine = 6;
      button = 6;
      break;
      case 0x20DFE817://7
      clearLights();
      currentRoutine = 7;
      button = 7;
      break;
      case 0x20DF18E7://8
      clearLights();
      currentRoutine = 8;
      button = 8;
      break;
      case 0x20DF9867://9
      button = 9;
      randomLight();
      break;
      case 0xFFFFFFFF://NULL
      break;
      case 0x20DF08F7://0
      button = 0;
      break;
      case 0x20DF807F:
      --calib;
      break;
      case 0x20DF00FF:
      ++calib;
      break;
      case 0x20DF22DD:
      calib = 5;
      break;
      default:
      button = -128;
      //DEBUG
      Serial.print("\r\nIR: ");
      Serial.print(results.value, HEX);
      Serial.print("\r\nilc> ");
      break;
    }
    irrecv.resume();
  }
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
    sensorLastTime = millis();
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
    case 2:
    routineTwo();
    break;
    case 3:
    routineThree();
    break;
    case 4:
    routineFour();
    break;
    case 5:
    routineFive();
    break;
    case 6:
    routineSix();
    break;
    case 7:
    routineSeven();
    break;
    case 8:
    routineEight();
    break;
  }
}

void routineOne() {
  if (lightCurrent == 0) {
    randomLight();
  }
  sensorNow = millis();
  if (sensorNow >= sensorLastTime + 25) {
    (void)analogRead(sensorCurrent);
    readings[0] = analogRead(sensorCurrent);
    (void)analogRead(sensorCurrent);
    readings[1] = analogRead(sensorCurrent);
    (void)analogRead(sensorCurrent);
    readings[2] = analogRead(sensorCurrent);
    (void)analogRead(sensorCurrent);
    readings[3] = analogRead(sensorCurrent);
    (void)analogRead(sensorCurrent);
    readings[4] = analogRead(sensorCurrent);
    sensorReading = 0;
    for(int i = 0; i < 5; ++i) {
      // Serial.print(i+1);
      // Serial.print(":");
      // Serial.print(readings[i]);
      // Serial.print("\t");
      sensorReading += readings[i];
    }
    sensorReading = sensorReading / 5;
    Serial.print("\r");
    Serial.print(calib);
    Serial.print("        ");
    // Serial.println();
    // Serial.print("avg");
    // Serial.print(sensorCurrent);
    // Serial.print(": ");
    // Serial.println(sensorReading);
    if (sensorReading < sensorLastReading - calib) {
      Serial.println();
      randomLight();
    }
    sensorLastTime = sensorNow;
    sensorLastReading = sensorReading;
  }
  // if (sensorNow%100 == 0) {
  //   Serial.print("\rSensor A");
  //   Serial.print(sensorCurrent);
  //   Serial.print(": ");
  //   Serial.print(sensorReading);
  // }
}

void routineTwo() {
  if (routineEnd == 0) {
    routineEnd = millis() + 60000;
  }
  if (millis() > routineEnd) {
    clearLights();
    routineEnd = 0;
    currentRoutine = 0;
    return;
  }
  if (!routineLightOn) {
    do {
      randomLight();
    } while (lightCurrent == lightLastOn);
    lightLastOn = lightCurrent;
    routineLightEnd = millis() + 1000;
    routineLightOn = 1;
  } else {
    if (millis() > routineLightEnd) {
      routineLightOn = 0;
    }
  }
}

void routineThree() {
  if (routineEnd == 0) {
    routineEnd = millis() + 60000;
    lightCurrent = random(lightFirst, lightLast+1);
    switch(lightCurrent) {
      case 2:
      sensorCurrent = 1;
      break;
      case 3:
      sensorCurrent = 0;
      break;
      case 4:
      sensorCurrent = 3;
      break;
      case 5:
      sensorCurrent = 6;
      break;
      case 6:
      sensorCurrent = 4;
      break;
      case 7:
      sensorCurrent = 2;
      break;
      case 8:
      sensorCurrent = 7;
      break;
      case 9:
      sensorCurrent = 5;
      break;
    }
  }
  if (millis() > routineEnd) {
    clearLights();
    routineEnd = 0;
    currentRoutine = 0;
    return;
  }
  if (!routineLightOn) {
    if (millis() > routineLightEndEnd) {
      digitalWrite(lightCurrent, HIGH);
      lightLastOn = lightCurrent;
      routineLightEnd = millis() + 2000;
      routineLightOn = 1;
    }
  } else {
    if (millis() > routineLightEnd) {
      routineLightOn = 0;
      routineLightEndEnd = millis() + 2000;
      digitalWrite(lightCurrent, LOW);
      do {
        lightCurrent = random(lightFirst, lightLast+1);
        switch(lightCurrent) {
          case 2:
          sensorCurrent = 1;
          break;
          case 3:
          sensorCurrent = 0;
          break;
          case 4:
          sensorCurrent = 3;
          break;
          case 5:
          sensorCurrent = 6;
          break;
          case 6:
          sensorCurrent = 4;
          break;
          case 7:
          sensorCurrent = 2;
          break;
          case 8:
          sensorCurrent = 7;
          break;
          case 9:
          sensorCurrent = 5;
          break;
        }
      } while (lightCurrent == lightLastOn);
    }
  }
}

void routineFour() {
  if (routineEnd == 0) {
    routineEnd = millis() + 120000;
    lightCurrent = random(lightFirst, lightLast+1);
    switch(lightCurrent) {
      case 2:
      sensorCurrent = 1;
      break;
      case 3:
      sensorCurrent = 0;
      break;
      case 4:
      sensorCurrent = 3;
      break;
      case 5:
      sensorCurrent = 6;
      break;
      case 6:
      sensorCurrent = 4;
      break;
      case 7:
      sensorCurrent = 2;
      break;
      case 8:
      sensorCurrent = 7;
      break;
      case 9:
      sensorCurrent = 5;
      break;
    }
  }
  if (millis() > routineEnd) {
    clearLights();
    routineEnd = 0;
    currentRoutine = 0;
    return;
  }
  if (!routineLightOn) {
    if (millis() > routineLightEndEnd) {
      digitalWrite(lightCurrent, HIGH);
      lightLastOn = lightCurrent;
      routineLightEnd = millis() + 3000;
      routineLightOn = 1;
    }
  } else {
    if (millis() > routineLightEnd) {
      routineLightOn = 0;
      routineLightEndEnd = millis() + 1000;
      digitalWrite(lightCurrent, LOW);
      do {
        lightCurrent = random(lightFirst, lightLast+1);
        switch(lightCurrent) {
          case 2:
          sensorCurrent = 1;
          break;
          case 3:
          sensorCurrent = 0;
          break;
          case 4:
          sensorCurrent = 3;
          break;
          case 5:
          sensorCurrent = 6;
          break;
          case 6:
          sensorCurrent = 4;
          break;
          case 7:
          sensorCurrent = 2;
          break;
          case 8:
          sensorCurrent = 7;
          break;
          case 9:
          sensorCurrent = 5;
          break;
        }
      } while (lightCurrent == lightLastOn);
    }
  }
}

void routineFive() {
  if (routineEnd == 0) {
    routineEnd = millis() + 120000;
    lightCurrent = random(lightFirst, lightLast+1);
    switch(lightCurrent) {
      case 2:
      sensorCurrent = 1;
      break;
      case 3:
      sensorCurrent = 0;
      break;
      case 4:
      sensorCurrent = 3;
      break;
      case 5:
      sensorCurrent = 6;
      break;
      case 6:
      sensorCurrent = 4;
      break;
      case 7:
      sensorCurrent = 2;
      break;
      case 8:
      sensorCurrent = 7;
      break;
      case 9:
      sensorCurrent = 5;
      break;
    }
  }
  if (millis() > routineEnd) {
    clearLights();
    routineEnd = 0;
    currentRoutine = 0;
    return;
  }
  if (!routineLightOn) {
    if (millis() > routineLightEndEnd) {
      digitalWrite(lightCurrent, HIGH);
      lightLastOn = lightCurrent;
      if (lightCurrent == 3 || lightCurrent == 4) {
        routineLightEnd = millis() + 250;
      } else {
        routineLightEnd = millis() + 1000;
      }
      routineLightOn = 1;
    }
  } else {
    if (millis() > routineLightEnd) {
      routineLightOn = 0;
      routineLightEndEnd = millis() + 500;
      digitalWrite(lightCurrent, LOW);
      do {
        lightCurrent = random(lightFirst, lightLast+1);
        switch(lightCurrent) {
          case 2:
          sensorCurrent = 1;
          break;
          case 3:
          sensorCurrent = 0;
          break;
          case 4:
          sensorCurrent = 3;
          break;
          case 5:
          sensorCurrent = 6;
          break;
          case 6:
          sensorCurrent = 4;
          break;
          case 7:
          sensorCurrent = 2;
          break;
          case 8:
          sensorCurrent = 7;
          break;
          case 9:
          sensorCurrent = 5;
          break;
        }
      } while (lightCurrent == lightLastOn);
    }
  }
}

void routineSix() {
  if (routineEnd == 0) {
    routineEnd = millis() + 120000;
    lightCurrent = random(lightFirst, lightLast+1);
    switch(lightCurrent) {
      case 2:
      sensorCurrent = 1;
      break;
      case 3:
      sensorCurrent = 0;
      break;
      case 4:
      sensorCurrent = 3;
      break;
      case 5:
      sensorCurrent = 6;
      break;
      case 6:
      sensorCurrent = 4;
      break;
      case 7:
      sensorCurrent = 2;
      break;
      case 8:
      sensorCurrent = 7;
      break;
      case 9:
      sensorCurrent = 5;
      break;
    }
  }
  if (millis() > routineEnd) {
    clearLights();
    routineEnd = 0;
    currentRoutine = 0;
    return;
  }
  if (!routineLightOn) {
    if (millis() > routineLightEndEnd) {
      digitalWrite(lightCurrent, HIGH);
      lightLastOn = lightCurrent;
      if (lightCurrent == 3 || lightCurrent == 4) {
        routineLightEnd = millis() + 250;
      } else {
        routineLightEnd = millis() + 2000;
      }
      routineLightOn = 1;
    }
  } else {
    if (millis() > routineLightEnd) {
      routineLightOn = 0;
      routineLightEndEnd = millis() + 500;
      digitalWrite(lightCurrent, LOW);
      do {
        lightCurrent = random(lightFirst, lightLast+1);
        switch(lightCurrent) {
          case 2:
          sensorCurrent = 1;
          break;
          case 3:
          sensorCurrent = 0;
          break;
          case 4:
          sensorCurrent = 3;
          break;
          case 5:
          sensorCurrent = 6;
          break;
          case 6:
          sensorCurrent = 4;
          break;
          case 7:
          sensorCurrent = 2;
          break;
          case 8:
          sensorCurrent = 7;
          break;
          case 9:
          sensorCurrent = 5;
          break;
        }
      } while (lightCurrent == lightLastOn);
    }
  }
}

void routineSeven() {
  if (routineEnd == 0) {
    routineEnd = millis() + 120000;
    lightCurrent = random(lightFirst, lightLast+1);
    switch(lightCurrent) {
      case 2:
      sensorCurrent = 1;
      break;
      case 3:
      sensorCurrent = 0;
      break;
      case 4:
      sensorCurrent = 3;
      break;
      case 5:
      sensorCurrent = 6;
      break;
      case 6:
      sensorCurrent = 4;
      break;
      case 7:
      sensorCurrent = 2;
      break;
      case 8:
      sensorCurrent = 7;
      break;
      case 9:
      sensorCurrent = 5;
      break;
    }
  }
  if (millis() > routineEnd) {
    clearLights();
    routineEnd = 0;
    currentRoutine = 0;
    return;
  }
  if (!routineLightOn) {
    if (millis() > routineLightEndEnd) {
      digitalWrite(lightCurrent, HIGH);
      lightLastOn = lightCurrent;
      if (lightCurrent == 3 || lightCurrent == 4) {
        routineLightEnd = millis() + 250;
      } else {
        routineLightEnd = millis() + 500;
      }
      routineLightOn = 1;
    }
  } else {
    if (millis() > routineLightEnd) {
      routineLightOn = 0;
      routineLightEndEnd = millis() + 500;
      digitalWrite(lightCurrent, LOW);
      do {
        lightCurrent = random(lightFirst, lightLast+1);
        switch(lightCurrent) {
          case 2:
          sensorCurrent = 1;
          break;
          case 3:
          sensorCurrent = 0;
          break;
          case 4:
          sensorCurrent = 3;
          break;
          case 5:
          sensorCurrent = 6;
          break;
          case 6:
          sensorCurrent = 4;
          break;
          case 7:
          sensorCurrent = 2;
          break;
          case 8:
          sensorCurrent = 7;
          break;
          case 9:
          sensorCurrent = 5;
          break;
        }
      } while (lightCurrent == lightLastOn);
    }
  }
}

void routineEight() {
  if (routineEnd == 0) {
    routineEnd = millis() + 120000;
    lightCurrent = 3;
  }
  if (millis() > routineEnd) {
    clearLights();
    routineEnd = 0;
    currentRoutine = 0;
    return;
  }
  if (!routineLightOn) {
    if (millis() > routineLightEndEnd) {
      digitalWrite(lightCurrent, HIGH);
      routineLightEnd = millis() + 500;
      routineLightOn = true;
    }
  } else {
    if (millis() > routineLightEnd) {
      routineLightOn = false;
      if (lightCurrent == 4) {
        clearLights();
        delay(2000);
        lightCurrent = 0;
      } else {
        switch(lightCurrent) {
          case 3:
          lightCurrent = 2;
          break;
          case 2:
          lightCurrent = 5;
          break;
          case 5:
          lightCurrent = 6;
          break;
          case 6:
          lightCurrent = 7;
          break;
          case 7:
          lightCurrent = 8;
          break;
          case 8:
          lightCurrent = 9;
          break;
          case 9:
          lightCurrent = 4;
          break;
          case 0:
          lightCurrent = 3;
          break;
        }
      }
      routineLightEndEnd = millis();
      digitalWrite(lightCurrent, LOW);
    }
  }
}

void randomLight() {
  Serial.print("sensorReading: ");
  Serial.println(sensorReading);
  Serial.print("sensorLastReading: ");
  Serial.println(sensorLastReading);
  digitalWrite(lightCurrent, LOW);
  lightCurrent = random(lightFirst, lightLast+1);
  switch(lightCurrent) {
    case 2:
    sensorCurrent = 1;
    break;
    case 3:
    sensorCurrent = 0;
    break;
    case 4:
    sensorCurrent = 3;
    break;
    case 5:
    sensorCurrent = 6;
    break;
    case 6:
    sensorCurrent = 4;
    break;
    case 7:
    sensorCurrent = 2;
    break;
    case 8:
    sensorCurrent = 7;
    break;
    case 9:
    sensorCurrent = 5;
    break;
  }
  Serial.print("\r\nSwitched light to ");
  Serial.print(lightCurrent);
  Serial.print("\r\n");
  delay(50);
  digitalWrite(lightCurrent, HIGH);
  delay(25);
  sensorLastReading = 0;
}

void clearLights() {
  Serial.println("clearing...");
  routineEnd = 0;
  lightCurrent = 0;
  for (int i = 2; i < 10; i++) {
    digitalWrite(i, LOW);
    delay(10);
  }
}

void checkVoltage() {
  if (millis()%1000 == 0) {
    voltage = analogRead(8)*5.0/1023.0*(R1+R2)/R2;
    Serial.print("\rV: ");
    Serial.print(voltage);
    if (voltage >= 13.8) {
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, HIGH);
      digitalWrite(redLed, HIGH);
    } else if (voltage >= 11.9 && voltage < 13.8) {
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, LOW);
      digitalWrite(redLed, LOW);
    } else if (voltage < 11.9 && voltage >= 11.0) {
      digitalWrite(greenLed, LOW);
      digitalWrite(yellowLed, HIGH);
      digitalWrite(redLed, LOW);
    } else if (voltage < 11.0) {
      digitalWrite(greenLed, LOW);
      digitalWrite(yellowLed, LOW);
      digitalWrite(redLed, HIGH);
    }
  }
}
