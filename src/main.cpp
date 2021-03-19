#include <Arduino.h>
#include "AccelStepper.h"

#define fanPin 4
#define dirPin 21
#define stepPin 15
#define enablePin 14
#define stopPin 18
#define ledPin 12
#define bigledPin 19
#define motorStepsPerRevolution 200
#define motorMicroSteps 16
#define tableRPM 3
const int tableStepsPerRevolution = motorStepsPerRevolution * motorMicroSteps;
const long distancePerMove = 500 * (long)tableStepsPerRevolution;

AccelStepper tableMotor(1,stepPin, dirPin);

void setup() {
  pinMode(fanPin, OUTPUT);
  pinMode(ledPin, OUTPUT); 
  pinMode(bigledPin, OUTPUT);
  pinMode(stopPin, INPUT);

  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);

  tableMotor.setMaxSpeed(tableStepsPerRevolution * ((float)tableRPM / 60));
  tableMotor.setAcceleration(tableStepsPerRevolution * ((float)tableRPM / 60));
  tableMotor.move(100000);

}

void loop() {
  if (!tableMotor.isRunning()) {
    if (abs(tableMotor.currentPosition()) == distancePerMove) {
      tableMotor.move(-1 * tableMotor.currentPosition());
    } else {
      tableMotor.move(copysign(distancePerMove, tableMotor.currentPosition()));
    }

  }

  if (digitalRead(stopPin)) {
    analogWrite(fanPin, 0);
    analogWrite(ledPin, 0);
    analogWrite(bigledPin, 0);
    tableMotor.stop();
  } else {
    analogWrite(fanPin, 255);
    analogWrite(ledPin, 255);
    analogWrite(bigledPin, 255);
    tableMotor.run();

  }
}