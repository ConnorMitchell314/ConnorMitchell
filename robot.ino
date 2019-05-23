#include <SoftwareSerial.h>
#include <AFMotor.h>
#include <SimpleSoftwareServo.h>
//#include <time.h>

SoftwareSerial bluetooth(50, 51); //rx, tx

//float const rootTwo = sqrt(2);

//motor initilization
AF_DCMotor mFrontRight(1);
AF_DCMotor mBackRight(2);
AF_DCMotor mBackLeft(3);
AF_DCMotor mFrontLeft(4);

SimpleSoftwareServo climbServo;
SimpleSoftwareServo armStage1Servo;
SimpleSoftwareServo armStage2Servo;
SimpleSoftwareServo intakeArticulationServo;

//Driver Station inputs
float leftXAxis;
float leftYAxis;
float trigger;
float climbAxis;
//float aButton;

void setup() {
    bluetooth.begin(9600);
    climbServo.attach(9);
    armStage1Servo.attach(10);
    //armStage2Servo.attach(11); // pin is a placeholder
    //intakeArticulationServo.attach(12); //pin is a placeholder
}

void loop() {
    //Driver station inputs
    while (bluetooth.available() > 0) {
        if ((bluetooth.read()) == 'z') {
            leftXAxis = (bluetooth.parseFloat());
            leftYAxis = (bluetooth.parseFloat());
            trigger = (bluetooth.parseFloat());
            climbAxis = bluetooth.parseFloat();
            //aButton = bluetooth.parseFloat();

            xDrive(leftXAxis, -leftYAxis, -trigger);
            if (climbAxis == 1) {
                climbServo.write(0);
            } else if (climbAxis == -1) {
                climbServo.write(180);
            } else {
                climbServo.write(55);
            }
            //SimpleSoftwareServo::refresh();
            // if (leftYAxis > 0.5) {
            //     mBackLeft.run(FORWARD);
            //     mBackLeft.setSpeed(255);
            // } else {
            //     mBackLeft.run(FORWARD);
            //     mBackLeft.setSpeed(0);
            // }
            
        } 
        SimpleSoftwareServo::refresh();
    }
  
}

void xDrive(double translateX, double translateY, float unregulatedRotation) {
    // double translationAngle;
    // if (translateX = 0) {
    //     if (translateY > 0) {
    //         translationAngle = 90;
    //     } else {
    //         translationAngle = -90;
    //     }
    // } else {
    //     translationAngle = atan(translateY / translateX);
    //     if (translateX <= 0) {
    //         if (translateY > 0) {
    //             translationAngle -= 180;
    //         } else {
    //             translationAngle += 180;
    //         }
    //     }
    // }
    // double translationMagnitude = sqrt(pow(translateX, 2) + pow(translateY, 2));
    // double translationPower [4] = {translationMagnitude * cos(translationAngle + 45), translationMagnitude * cos(translationAngle - 45), 
    //                                 translationMagnitude * cos(translationAngle - 135), translationMagnitude * cos(translationAngle + 135)};
    // double finalPower [4] = {translationPower[0] + unregulatedRotation, translationPower[1] + unregulatedRotation, 
    //                         translationPower[2] + unregulatedRotation, translationPower[3] + unregulatedRotation};
    // double maxPower = abs(finalPower[0]);
    // if (abs(finalPower[1]) > maxPower) maxPower = abs(finalPower[1]);
    // if (abs(finalPower[2]) > maxPower) maxPower = abs(finalPower[2]);
    // if (abs(finalPower[3]) > maxPower) maxPower = abs(finalPower[3]);
    // if (maxPower > 1) {
    //     for (int i = 0 ; i < 4 ; i ++) {
    //         finalPower[i] /= maxPower;
    //     }
    // }
    // mFrontRight.run((finalPower[0] >= 0) ? FORWARD : BACKWARD);
    // mFrontRight.setSpeed(abs(finalPower[0]) * 255);
    // mFrontLeft.run((finalPower[1] >= 0) ? FORWARD : BACKWARD);
    // mFrontLeft.setSpeed(abs(finalPower[1]) * 255);
    // mBackLeft.run((finalPower[2] >= 0) ? FORWARD : BACKWARD);
    // mBackLeft.setSpeed(abs(finalPower[2]) * 255);
    // mBackRight.run((finalPower[3] >= 0) ? FORWARD : BACKWARD);
    // mBackRight.setSpeed(abs(finalPower[3]) * 255);
    if (abs(translateX) > 0.3 || abs(translateY) > 0.3) {

        if (translateX > 0.3) {
            mFrontLeft.run(FORWARD);
            mFrontLeft.setSpeed((translateX * 255) + unregulatedRotation);
            mBackRight.run(FORWARD);
            mBackRight.setSpeed((translateX * 255) + unregulatedRotation);
        } else if (translateX < -0.3) {
            mFrontLeft.run(BACKWARD);
            mFrontLeft.setSpeed((translateX * -255) + unregulatedRotation);
            mBackRight.run(BACKWARD);
            mBackRight.setSpeed((translateX * -255) + unregulatedRotation);
        } else {
            mFrontLeft.setSpeed(0);
            mBackRight.setSpeed(0);
        }
        if (translateY > 0.3) {
            mBackLeft.run(FORWARD);
            mBackLeft.setSpeed((translateX * 255) + unregulatedRotation);
            mFrontRight.run(FORWARD);
            mFrontRight.setSpeed((translateX * 255) + unregulatedRotation);
        } else if (translateY < -0.3) {
            mBackLeft.run(BACKWARD);
            mBackLeft.setSpeed((translateX * -255) + unregulatedRotation);
            mFrontRight.run(BACKWARD);
            mFrontRight.setSpeed((translateX * -255) + unregulatedRotation);
        } else {
            mBackLeft.setSpeed(0);
            mFrontRight.setSpeed(0);
        }
    } else {
        mFrontRight.run((unregulatedRotation >= 0) ? FORWARD : BACKWARD);
        mFrontRight.setSpeed(abs(unregulatedRotation) * 255);
        mFrontLeft.run((unregulatedRotation >= 0) ? FORWARD : BACKWARD);
        mFrontLeft.setSpeed(abs(unregulatedRotation) * 255);
        mBackLeft.run((unregulatedRotation <= 0) ? FORWARD : BACKWARD);
        mBackLeft.setSpeed(abs(unregulatedRotation) * 255);
        mBackRight.run((unregulatedRotation <= 0) ? FORWARD : BACKWARD);
        mBackRight.setSpeed(abs(unregulatedRotation) * 255);
    }
}
