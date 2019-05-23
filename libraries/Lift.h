#ifndef Lift_h
#define Lift_h

#include <Arduino.h>
#include <SimpleSoftwareServo.h>
#include <AFMotor.h>

class Lift {
    public:
        Lift(int liftMotorPin, int armStage1Pin, int armStage2Pin);
        void stopLift();
        enum positions {CARGO_INTAKE, HATCH_INTAKE, CARGO_SHIP, ROCKET_LEVEL_1, ROCKET_LEVEL_2, ROCKET_LEVEL_3, CUSTOM};
        void setPosition(positions newPosition);
        void customLiftPower(int customLiftPower);
        void customArmPosition(int customArmPosition);
        void customEndPosition(int customEndPosition);
    private:
        positions liftPositions;
        boolean liftStopped;
        void updateLift();
        AF_DCMotor liftMotor
        SimpleSoftwareServo armStage1
        SimpleSoftwareServo armStage2
        int _liftMotorPin, _armStage1Pin, _armStage2Pin;
};
#endif