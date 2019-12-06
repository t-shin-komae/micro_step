#include <Stepper.h>
#include <stdint.h>
#include <new.h>
#include "microStepper.h"
const uint16_t stepsPerRevolution = 200;
// Stepper myStepper(stepsPerRevolution,8,9,10,11);

microStepper myStepper(stepsPerRevolution, 8, 9, 10, 11);


void setup() {
    Serial.begin(9600);
}

void loop() {
    myStepper.setSpeed(30);
    myStepper.micro_step(200);
    delay(5000);
    myStepper.setSpeed(30);
    myStepper.step(200);
    delay(5000);
}
