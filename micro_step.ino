#include <Stepper.h>
#include <stdint.h>
#include <new.h>
#include "microStepper.h"
const uint16_t stepsPerRevolution = 200;
// Stepper myStepper(stepsPerRevolution,8,9,10,11);

microStepper myStepper(stepsPerRevolution, 16, 8, 9, 10, 11);


void setup() {
    Serial.begin(115200);
}

void loop() {
    myStepper.setSpeed(5);
    myStepper.micro_step(20,true);
    delay(1000);
    myStepper.setSpeed(60);
    myStepper.micro_step(200,true);
    delay(1000);
    myStepper.setSpeed(240);
    myStepper.micro_step(200,true);
    delay(1000);
}
