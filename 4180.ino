#include <Servo.h>

#define ledPin LED_BUILTIN

const int smallLoadBtnPin = 0;
const int mediumLoadBtnPin =  1;
const int largeLoadBtnPin = 2;

const int ain2 = 3;
const int ain1 = 4;
const int pwmPin = 5;

const int solenoidPin = 6;

Servo primaryServo;
Servo secondaryServo;

// -1 = Backwards; 0 = Stop; 1 = Forwards
void setMotors(int value) {
  //digitalWrite(ain1, LOW);
  //digitalWrite(ain2, LOW);
  switch (value) {
    case 0:
      digitalWrite(ain1, LOW);
      digitalWrite(ain2, LOW);
      break;
    case -1:
      digitalWrite(ain1, LOW);
      digitalWrite(ain2, HIGH);
      break;
    case 1:
      digitalWrite(ain1, HIGH);
      digitalWrite(ain2, LOW);
      break;
    default:
      digitalWrite(ain1, LOW);
      digitalWrite(ain2, LOW);
      break;
  }
}

// 3 buttons small, medium, light
// 2 servos
// pump - Hbridge - only needed for bi-directional support
// Solenoid - mosfet

// duration - how long to run the pump (in milliseconds)
void dispenseLoad(int duration) {
  digitalWrite(ledPin, LOW);
  // Set servos to fully extended
  primaryServo.write(200);
  secondaryServo.write(0);

  //Open Solenoid
  digitalWrite(solenoidPin, HIGH);

  delay(500); // Wait for the arms to extend before starting pump
  //Turn on Motor in the forward direction
  setMotors(1);

  delay(duration);

  // Clean up

  setMotors(0);

  delay(250); // Don't immediately reverse the pump

  setMotors(-1);
  delay(500);
  primaryServo.write(10);
  secondaryServo.write(180);
  delay(7500);
  setMotors(0);
  digitalWrite(solenoidPin, LOW);

  digitalWrite(ledPin, HIGH);
}

void setup() {
  pinMode(smallLoadBtnPin, INPUT_PULLUP);
  pinMode(mediumLoadBtnPin, INPUT_PULLUP);
  pinMode(largeLoadBtnPin, INPUT_PULLUP);

  primaryServo.attach(8);
  secondaryServo.attach(9);
  primaryServo.write(10);
  secondaryServo.write(180);

  pinMode(ledPin, OUTPUT);

  
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(pwmPin, OUTPUT);

  digitalWrite(pwmPin, HIGH);
  //analogWrite(pwmPin, 80);
  setMotors(0); // stop;
  
  pinMode(solenoidPin, OUTPUT);
}

void loop() {
  if (!digitalRead(smallLoadBtnPin)) {
    dispenseLoad(10000);
  } else if (!digitalRead(mediumLoadBtnPin)) {
    dispenseLoad(30000);
  } else if (!digitalRead(largeLoadBtnPin)) {
    dispenseLoad(60000);
  }
  delay(100);
}
