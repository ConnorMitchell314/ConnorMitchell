#include <SoftwareSerial.h>
#include <AFMotor.h>
//#include <SimpleSoftwareServo.h>
#include <Servo.h>
#include <Lift.h> 
//#include <time.h>

SoftwareSerial bluetooth(51, 50); //rx, tx
//Lift lift(40, 41, 38, 39);
//float const rootTwo = sqrt(2);

//motor initilization
AF_DCMotor mFrontRight(1);
AF_DCMotor mBackRight(2);
AF_DCMotor mBackLeft(3);
AF_DCMotor mFrontLeft(4);

Servo climbServo;
Servo armStage1Servo;
Servo armStage2Servo;
Servo intakeArticulationServo;

//Driver Station inputs
float rightXAxis;
float rightYAxis;
float trigger;
float climbButtons;
float leftXAxis;
float leftYAxis;
float leftBumper;
float rightBumper;
float autoRuns1and2;
float autoRuns3and4;
float aButton;
float bButton;
float autoRuns5and6;

int liftMotorPinA = 40;
int liftMotorPinB = 41;
int intakeMotorPinA = 38;
int intakeMotorPinB = 39;
int encoderPinA = 53;
int encoderPinB = 52;

int encoder0Pos = 0;
double liftGoalPos = 10;
int encoder0PinALast = LOW;
int n = LOW;
float armPosition = 45;

int intakeState = 0;

void setup() {
    bluetooth.begin(9600);
    //climbServo.attach(9);
    armStage1Servo.attach(10);
    //armStage2Servo.attach(A3); // pin is a placeholder
    intakeArticulationServo.attach(A3); //pin is a placeholder
    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);

}

void loop() {
    while (bluetooth.available() > 0) {
        if ((bluetooth.read()) == 'z') {
            rightXAxis = (bluetooth.parseFloat());
            rightYAxis = (bluetooth.parseFloat());
            trigger = (bluetooth.parseFloat());
            leftXAxis = bluetooth.parseFloat();
            leftYAxis = bluetooth.parseFloat();
            climbButtons = bluetooth.parseFloat();
            leftBumper = bluetooth.parseFloat();
            rightBumper = bluetooth.parseFloat();
            autoRuns1and2 = bluetooth.parseFloat();
            autoRuns3and4 = bluetooth.parseFloat();
            aButton = bluetooth.parseFloat();
            bButton = bluetooth.parseFloat();
            autoRuns5and6 = bluetooth.parseFloat();

            if (autoRuns1and2 == 1) {
                xDrive(-1, 1, 0);
                delay(2000);
                xDrive(0, 0, 0);
            } else if (autoRuns1and2 == -1) {
                xDrive(1, 1, 0);
                delay(2000);
                xDrive(0, 0, 0);
            } else if (autoRuns3and4 == 1) {
                xDrive(0, 1, 0);
                delay(3000);
                xDrive(0, 0, 0);
            } else if(autoRuns3and4 == -1) {
                xDrive(0, -1, 0);
                delay(3000);
                // xDrive(0, 0, -1);
                // delay(250);
                // xDrive(0, 1, 0);
                // delay(2000);
                xDrive(0, 0, 0);
            } else if(autoRuns5and6 == 1) {
                xDrive(0, -0.8, 0);
                delay(1000);
                climbControl(-1);
                delay(1000);
                climbControl(0);
                delay(1500);
                xDrive(0, 0, 0);
            }

            //aButton = bluetooth.parseFloat();
            xDrive(rightXAxis, -rightYAxis, -trigger);
            liftControl(leftYAxis, rightBumper, leftBumper);
            //climbControl(climbButtons);
            climbControl(aButton - bButton);
            intakeControl(aButton, bButton);
          
            
            //SimpleSoftwareServo::refresh();
        } 
        
    }
  
}

void climbControl(int climbButtons) {
    if (climbButtons == 1) {
        climbServo.attach(9);
        climbServo.write(180);
    } else if (climbButtons == -1) {
        climbServo.attach(9);
        climbServo.write(0);
     } //else if (climbButtons == -0.2) {
    //     climbServo.attach(9);
    //     climbServo.write(45); }
     else {
        climbServo.detach();
    }
}
void liftControl(int leftYAxis, int rightBumper, int leftBumper) {
    //liftGoalPos += 0.1 * leftYAxis; 
    runLiftMotor(leftYAxis);
    // if (leftBumper == 1) {
    //     armPosition = 110;
    // }
    // if (rightBumper == 1) {
    //     armPosition == 10;
    // }
    // armPosition += 3 * bumperSum;
    // if (armPosition < 5) {
    //     armPosition = 5;
    // } else if (armPosition > 180) {
    //     armPosition = 180;
    // }
    //armStage1Servo.write(armPosition);
    //armStage2Servo.write(armPosition - 5);
    // if (liftGoalPos < 100 && leftYAxis < 0) {
    //     armStage1Servo
    // } (liftGoalPos - liftEncoder()) / 50
}

int liftEncoder() {
    n = digitalRead(encoderPinA);
    if ((encoder0PinALast == LOW) && (n == HIGH)) {
        if (digitalRead(encoderPinB) == LOW) {
            encoder0Pos--;

        } else {
            encoder0Pos++;
        }
    }
    encoder0PinALast = n;
    return encoder0Pos;
}

void runLiftMotor(int liftMotorSpeed) {
    if (liftMotorSpeed < -1) {
        liftMotorSpeed = -1;
    } else if (liftMotorSpeed > 1) {
        liftMotorSpeed = 1;
    }

    if (liftMotorSpeed > 0) {
        digitalWrite(40, LOW);
        analogWrite(41, liftMotorSpeed * 240);
    } else {
        digitalWrite(41, LOW);
        analogWrite(40, -liftMotorSpeed * 240);
    }
}

void intakeControl(int close, int open) {
    if (close == 1) {
        intakeState = 1;
    } else if (open == 1) {
        intakeState = -1;
    }

    if (intakeState == 1) {
        intakeArticulationServo.write(180);
    } else if (intakeState == -1){
        intakeArticulationServo.write(0);
    } else {
        intakeArticulationServo.write(90);
    }
}

void xDrive(double translateX, double translateY, float unregulatedRotation) {
    translateX *= 255;

    translateY *= 255;

    double translationAngle;
    if (translateX == 0) {
        if (translateY > 0) {
            translationAngle = 90;
        } else {
            translationAngle = -90;
        }
    } else {
        translationAngle = (180 / PI) * atan(translateY / translateX);
        if (translateX < 0) {
            if (translateY > 0) {
                translationAngle -= 180;
            } else {

                translationAngle += 180;
            }
        }
    }
    double translationMagnitude = sqrt((translateX * translateX) + (translateY * translateY));
    unregulatedRotation *= 300 - translationMagnitude;
    double translationPower[4] = {translationMagnitude * cos((translationAngle + 45) * (PI / 180)), translationMagnitude * cos((translationAngle - 45) * (PI / 180)), 
                                  translationMagnitude * cos((translationAngle - 135) * (PI / 180)), translationMagnitude * cos((translationAngle + 135) * (PI / 180))};
    double finalPower[4] = {translationPower[0] + unregulatedRotation, translationPower[1] + unregulatedRotation, translationPower[2] + unregulatedRotation, translationPower[3] + unregulatedRotation};
    double maxPower = abs(finalPower[0]);
    if (abs(finalPower[1]) > maxPower) maxPower = abs(finalPower[1]);
    if (abs(finalPower[2]) > maxPower) maxPower = abs(finalPower[2]);
    if (abs(finalPower[3]) > maxPower) maxPower = abs(finalPower[3]);
    if (maxPower > 255) {
        for (int i = 0 ; i < 4 ; i ++) {
            finalPower[i] /= maxPower;
            finalPower[i] *= 255;
        }
    }
    mFrontRight.run((finalPower[0] >= 0) ? FORWARD : BACKWARD);
    mFrontRight.setSpeed(abs(finalPower[0]));
    mFrontLeft.run((finalPower[1] >= 0) ? FORWARD : BACKWARD);
    mFrontLeft.setSpeed(abs(finalPower[1]));
    mBackLeft.run((finalPower[2] <= 0) ? FORWARD : BACKWARD);
    mBackLeft.setSpeed(abs(finalPower[2]));
    mBackRight.run((finalPower[3] <= 0) ? FORWARD : BACKWARD);
    mBackRight.setSpeed(abs(finalPower[3]));
}
