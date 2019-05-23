#include <Arduino.h>
#include <AFMotor.h>
#include <Lift.h>

Lift::Lift() {

}

void Lift::stopLift() {
    liftStopped = true;
}

void Lift::setPosition(positions newPosition) {
    liftPositions = newPosition;
}

void Lift::customLiftPower(int customLiftPower) {
    liftPositions = CUSTOM;
}

void Lift::customArmPosition(int customArmPosition) {

}

void Lift::customEndPosition(int customEndPosition) {

}

void Lift::updateLift() {

}