#include <IRremote.h>
//Pin Configuration
const uint8_t recvPin = 11;
//IR Configuration
IRrecv irrecv(recvPin);
decode_results results;
uint8_t button = NULL;
//Async blink
unsigned long offTime = 0;
uint8_t blinkPin = 13;
bool blinkOn = false;
//Serial
char serialBuffer[256];
char serialReading = '\0';
uint8_t serialIndex = 0;
//Lights
uint8_t lowest = 2;
uint8_t highest = 9;
uint8_t currentLight = NULL;
uint8_t lastLight = NULL;
uint8_t currentSensor = NULL;
int * currentCalib = NULL;
uint8_t currentRoutine = 0;
//PINs
int calib0 = 1023;
int calib1 = 1023;
int calib2 = 1023;
int calib3 = 1023;
int calib4 = 1023;
int calib5 = 1023;
int calib6 = 1023;
int calib7 = 1023;

void setup()
{
  Serial.begin(115200);
  pinMode(blinkPin, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  irrecv.enableIRIn();
  //MOVE THIS
  randomLight();
  calibrate();
}

void loop() {
  checkIr();
  checkBlink();
  checkSerial();
  doRoutine();
}

void checkIr() {
  if (irrecv.decode(&results)) {
    switch(results.value) {
      case 0x100BCBD://Power
      button = 10;
      currentRoutine = 0;
      digitalWrite(currentLight, LOW);
      break;
      case 0x1000809://1
      button = 1;
      digitalWrite(currentLight, HIGH);
      currentRoutine = 1;
      break;
      case 0x1008889://2
      currentRoutine = 2;
      button = 2;
      break;
      case 0x1004849://3
      currentRoutine = 3;
      button = 3;
      break;
      case 0x100C8C9://4
      currentRoutine = 4;
      button = 4;
      break;
      case 0x1002829://5
      currentRoutine = 5;
      button = 5;
      break;
      case 0x100A8A9://6
      currentRoutine = 6;
      button = 6;
      break;
      case 0x1006869://7
      currentRoutine = 7;
      button = 7;
      break;
      case 0x100E8E9://8
      currentRoutine = 8;
      button = 8;
      break;
      case 0x1001819://9
      button = 9;
      digitalWrite(currentLight, LOW);
      randomLight();
      digitalWrite(currentLight, HIGH);
      break;
      case 0x1009899://0
      calibrate();
      button = 0;
      break;
      default:
      blinkFor(250);
      button = NULL;
      //DEBUG
      Serial.print("IR: ");
      Serial.println(results.value, HEX);
      break;
    }
    Serial.print("Button: ");
    Serial.println(button);
    irrecv.resume();
  }
}

void blinkFor(int duration) {
  //DEBUG
  Serial.print("BLINK: ON, ");
  Serial.println(duration);
  offTime = millis() + duration;
  digitalWrite(blinkPin, HIGH);
  blinkOn = true;
}

void checkBlink() {
  if (millis() > offTime && blinkOn) {
    digitalWrite(blinkPin, LOW);
    blinkOn = false;
    //DEBUG
    Serial.println("BLINK: OFF");
  }
}

void calibrate() {
  int reading = 1023;
  //Calibrate A0
  Serial.print("\rCalibrating A0");
  for (int start = millis(); millis() < start+250;) {
    reading = analogRead(0);
    if (reading < calib0) {
      calib0 = reading;
    }
  }
  //Calibrate A1
  Serial.print("\rCalibrating A1");
  for (int start = millis(); millis() < start+250;) {
    reading = analogRead(1);
    if (reading < calib1) {
      calib1 = reading;
    }
  }
  //Calibrate A2
  Serial.print("\rCalibrating A2");
  for (int start = millis(); millis() < start+250;) {
    reading = analogRead(2);
    if (reading < calib2) {
      calib2 = reading;
    }
  }
  //Calibrate A3
  Serial.print("\rCalibrating A3");
  for (int start = millis(); millis() < start+250;) {
    reading = analogRead(3);
    if (reading < calib3) {
      calib3 = reading;
    }
  }
  //Calibrate A4
  Serial.print("\rCalibrating A4");
  for (int start = millis(); millis() < start+250;) {
    reading = analogRead(4);
    if (reading < calib4) {
      calib4 = reading;
    }
  }
  //Calibrate A5
  Serial.print("\rCalibrating A5");
  for (int start = millis(); millis() < start+250;) {
    reading = analogRead(5);
    if (reading < calib5) {
      calib5 = reading;
    }
  }
  //Calibrate A6
  Serial.print("\rCalibrating A6");
  for (int start = millis(); millis() < start+250;) {
    reading = analogRead(6);
    if (reading < calib6) {
      calib6 = reading;
    }
  }
  //Calibrate A7
  Serial.print("\rCalibrating A7");
  for (int start = millis(); millis() < start+250;) {
    reading = analogRead(7);
    if (reading < calib7) {
      calib7 = reading;
    }
  }
  Serial.println();
  //DEBUG
  Serial.println("CAL: {");
  Serial.print("  0: ");
  Serial.println(calib0);
  Serial.print("  1: ");
  Serial.println(calib1);
  Serial.print("  2: ");
  Serial.println(calib2);
  Serial.print("  3: ");
  Serial.println(calib3);
  Serial.print("  4: ");
  Serial.println(calib4);
  Serial.print("  5: ");
  Serial.println(calib5);
  Serial.print("  6: ");
  Serial.println(calib6);
  Serial.print("  7: ");
  Serial.println(calib7);
  Serial.println("}");
}

void randomLight() {
  //DEBUG
  currentLight = random(lowest, 9);
  switch (currentLight) {
    case 2:
    currentSensor = 4;
    currentCalib = &calib4;
    break;
    case 3:
    currentSensor = 1;
    currentCalib = &calib1;
    break;
    case 4:
    currentSensor = 3;
    currentCalib = &calib3;
    break;
    case 5:
    currentSensor = 2;
    currentCalib = &calib2;
    break;
    case 6:
    currentSensor = 7;
    currentCalib = &calib7;
    break;
    case 7:
    currentSensor = 6;
    currentCalib = &calib6;
    break;
    case 8:
    currentSensor = 5;
    currentCalib = &calib5;
    break;
    case 9:
    currentSensor = 3;
    currentCalib = &calib3;
    break;
  }
}

void doRoutine() {
  if (currentRoutine == 0) {
  } else if (currentRoutine == 1) {
    int reading = analogRead(currentSensor);
    if (millis()%100 == 0) {
      Serial.print("\rValue: ");
      Serial.print(reading);
    }
    // if (millis()%1000 == 0) {
    //   Serial.print("Sensor A");
    //   Serial.print(currentSensor);
    //   Serial.print(": ");
    //   Serial.println(analogRead(currentSensor));
    // }
    if (reading < *currentCalib-2) {
      digitalWrite(currentLight, LOW);
      Serial.print("Turned off ");
      Serial.println(currentLight);
      lastLight = currentLight;
      randomLight();
      if (currentLight == lastLight) {
        digitalWrite(currentLight, HIGH);
        delay(500);
      }
      digitalWrite(currentLight, HIGH);
      Serial.print("Turned on ");
      Serial.println(currentLight);
    }
  } else if (currentRoutine == 2) {
    button = 2;
    for (int start = millis(); millis() < start+60000;) {
      Serial.println(millis());
      checkIr();
      if (button == 10) {
        currentRoutine = 0;
        break;
      }
      lastLight = currentLight;
      currentLight = random(2, 9);
      while (currentLight == lastLight) {
        lastLight = currentLight;
        currentLight = random(2, 9);
      }
      digitalWrite(currentLight, HIGH);
      delay(1000);
      digitalWrite(currentLight, LOW);
    }
  } else if (currentRoutine == 3) {
    for (int start = millis(); millis() < start+60000;) {
      checkIr();
      if (button == 10) {
        currentRoutine = 0;
        break;
      }
      lastLight = currentLight;
      currentLight = random(2, 9);
      while (currentLight == lastLight) {
        lastLight = currentLight;
        currentLight = random(2, 9);
      }
      digitalWrite(currentLight, HIGH);
      delay(2000);
      digitalWrite(currentLight, LOW);
      delay(2000);
    }
  } else if (currentRoutine == 4) {
    for (int start = millis(); millis() < start+120000;) {
      checkIr();
      if (button == 10) {
        currentRoutine = 0;
        break;
      }
      lastLight = currentLight;
      currentLight = random(2, 9);
      while (currentLight == lastLight) {
        lastLight = currentLight;
        currentLight = random(2, 9);
      }
      digitalWrite(currentLight, HIGH);
      delay(3000);
      digitalWrite(currentLight, LOW);
      delay(1000);
    }
  } else if (currentRoutine == 5) {
    for (int start = millis(); millis() < start+120000;) {
      checkIr();
      if (button == 10) {
        currentRoutine = 0;
        break;
      }
      lastLight = currentLight;
      currentLight = random(2, 9);
      while (currentLight == lastLight) {
        lastLight = currentLight;
        currentLight = random(2, 9);
      }
      digitalWrite(currentLight, HIGH);
      if (currentLight == 7 || currentLight == 4) {
        delay(200);
      } else {
        delay(1000);
      }
      digitalWrite(currentLight, LOW);
      delay(500);
    }
  } else if (currentRoutine == 6) {
    for (int start = millis(); millis() < start+120000;) {
      checkIr();
      if (button == 10) {
        currentRoutine = 0;
        break;
      }
      lastLight = currentLight;
      currentLight = random(2, 9);
      while (currentLight == lastLight) {
        lastLight = currentLight;
        currentLight = random(2, 9);
      }
      digitalWrite(currentLight, HIGH);
      if (currentLight == 7 || currentLight == 4) {
        delay(200);
      } else {
        delay(2000);
      }
      digitalWrite(currentLight, LOW);
      delay(500);
    }
  } else if (currentRoutine == 7) {
    for (int start = millis(); millis() < start+120000;) {
      checkIr();
      if (button == 10) {
        currentRoutine = 0;
        break;
      }
      lastLight = currentLight;
      currentLight = random(2, 9);
      while (currentLight == lastLight) {
        lastLight = currentLight;
        currentLight = random(2, 9);
      }
      digitalWrite(currentLight, HIGH);
      if (currentLight == 7 || currentLight == 4) {
        delay(200);
      } else {
        delay(500);
      }
      digitalWrite(currentLight, LOW);
      delay(500);
    }
  } else if (currentRoutine == 8) {
    for (int start = millis(); millis() < start+120000;) {
      Serial.println(start+120000);
      checkIr();
      if (button == 10) {
        digitalWrite(2, LOW);
        delay(10);
        digitalWrite(3, LOW);
        delay(10);
        digitalWrite(4, LOW);
        delay(10);
        digitalWrite(5, LOW);
        delay(10);
        digitalWrite(6, LOW);
        delay(10);
        digitalWrite(7, LOW);
        delay(10);
        digitalWrite(8, LOW);
        delay(10);
        digitalWrite(9, LOW);
        currentRoutine = 0;
        break;
      }
      digitalWrite(4, HIGH);
      delay(500);
      checkIr();
      if (button == 10) {
        digitalWrite(2, LOW);
        delay(10);
        digitalWrite(3, LOW);
        delay(10);
        digitalWrite(4, LOW);
        delay(10);
        digitalWrite(5, LOW);
        delay(10);
        digitalWrite(6, LOW);
        delay(10);
        digitalWrite(7, LOW);
        delay(10);
        digitalWrite(8, LOW);
        delay(10);
        digitalWrite(9, LOW);
        currentRoutine = 0;
        break;
      }
      digitalWrite(2, HIGH);
      delay(500);
      checkIr();
      if (button == 10) {
        digitalWrite(2, LOW);
        delay(10);
        digitalWrite(3, LOW);
        delay(10);
        digitalWrite(4, LOW);
        delay(10);
        digitalWrite(5, LOW);
        delay(10);
        digitalWrite(6, LOW);
        delay(10);
        digitalWrite(7, LOW);
        delay(10);
        digitalWrite(8, LOW);
        delay(10);
        digitalWrite(9, LOW);
        currentRoutine = 0;
        break;
      }
      digitalWrite(3, HIGH);
      delay(500);
      checkIr();
      if (button == 10) {
        digitalWrite(2, LOW);
        delay(10);
        digitalWrite(3, LOW);
        delay(10);
        digitalWrite(4, LOW);
        delay(10);
        digitalWrite(5, LOW);
        delay(10);
        digitalWrite(6, LOW);
        delay(10);
        digitalWrite(7, LOW);
        delay(10);
        digitalWrite(8, LOW);
        delay(10);
        digitalWrite(9, LOW);
        currentRoutine = 0;
        break;
      }
      digitalWrite(5, HIGH);
      delay(500);
      checkIr();
      if (button == 10) {
        digitalWrite(2, LOW);
        delay(10);
        digitalWrite(3, LOW);
        delay(10);
        digitalWrite(4, LOW);
        delay(10);
        digitalWrite(5, LOW);
        delay(10);
        digitalWrite(6, LOW);
        delay(10);
        digitalWrite(7, LOW);
        delay(10);
        digitalWrite(8, LOW);
        delay(10);
        digitalWrite(9, LOW);
        currentRoutine = 0;
        break;
      }
      digitalWrite(6, HIGH);
      delay(500);
      checkIr();
      if (button == 10) {
        digitalWrite(2, LOW);
        delay(10);
        digitalWrite(3, LOW);
        delay(10);
        digitalWrite(4, LOW);
        delay(10);
        digitalWrite(5, LOW);
        delay(10);
        digitalWrite(6, LOW);
        delay(10);
        digitalWrite(7, LOW);
        delay(10);
        digitalWrite(8, LOW);
        delay(10);
        digitalWrite(9, LOW);
        currentRoutine = 0;
        break;
      }
      digitalWrite(8, HIGH);
      delay(500);
      checkIr();
      if (button == 10) {
        digitalWrite(2, LOW);
        delay(10);
        digitalWrite(3, LOW);
        delay(10);
        digitalWrite(4, LOW);
        delay(10);
        digitalWrite(5, LOW);
        delay(10);
        digitalWrite(6, LOW);
        delay(10);
        digitalWrite(7, LOW);
        delay(10);
        digitalWrite(8, LOW);
        delay(10);
        digitalWrite(9, LOW);
        currentRoutine = 0;
        break;
      }
      digitalWrite(9, HIGH);
      delay(500);
      checkIr();
      if (button == 10) {
        digitalWrite(2, LOW);
        delay(10);
        digitalWrite(3, LOW);
        delay(10);
        digitalWrite(4, LOW);
        delay(10);
        digitalWrite(5, LOW);
        delay(10);
        digitalWrite(6, LOW);
        delay(10);
        digitalWrite(7, LOW);
        delay(10);
        digitalWrite(8, LOW);
        delay(10);
        digitalWrite(9, LOW);
        currentRoutine = 0;
        break;
      }
      digitalWrite(7, HIGH);
      delay(2000);
      checkIr();
      if (button == 10) {
        digitalWrite(2, LOW);
        delay(10);
        digitalWrite(3, LOW);
        delay(10);
        digitalWrite(4, LOW);
        delay(10);
        digitalWrite(5, LOW);
        delay(10);
        digitalWrite(6, LOW);
        delay(10);
        digitalWrite(7, LOW);
        delay(10);
        digitalWrite(8, LOW);
        delay(10);
        digitalWrite(9, LOW);
        currentRoutine = 0;
        break;
      }
      digitalWrite(2, LOW);
      delay(10);
      digitalWrite(3, LOW);
      delay(10);
      digitalWrite(4, LOW);
      delay(10);
      digitalWrite(5, LOW);
      delay(10);
      digitalWrite(6, LOW);
      delay(10);
      digitalWrite(7, LOW);
      delay(10);
      digitalWrite(8, LOW);
      delay(10);
      digitalWrite(9, LOW);
      Serial.println(millis());
      delay(1000);
      Serial.println(millis());
    }
  }
}

void checkSerial() {
  if (Serial.available() > 0) {
    serialReading = Serial.read();
    if (serialReading == '\r') {
      Serial.println();
      serialBuffer[serialIndex] = '\0';
      parseCommand(serialBuffer);
      serialBuffer[0] = '\0';
      serialIndex = 0;
    } else {
      Serial.print(serialReading);
      serialBuffer[serialIndex] = serialReading;
      serialIndex++;
    }
  }
}

void parseCommand(char command[256]) {
}
