#include <Wire.h>
#include <SoftwareSerial.h>
#include <AFMotor.h>
//#include <SimpleSoftwareServo.h>
//#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <Lift.h> 
//#include <time.h>

SoftwareSerial bluetooth(51, 50); //rx, tx

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//motor initilization
AF_DCMotor dmLeft(1);
AF_DCMotor dmRight(2);
AF_DCMotor liftMotor(3);
AF_DCMotor intakeMotor(4);

//Servo climbServo;

//Servo arm;

//Driver Station inputs
float leftXAxis;
float leftYAxis;
float trigger;
float rightYAxis;
float rightXAxis;
int aButton;
int bButton;
int xButton;
int yButton;
int leftBumper;
int rightBumper;
int auto1, auto2, auto3, auto4, auto5;
int autoReset = 0;

#define SERVOMIN 270
#define SERVOMAX 530

const int liftDTPin = 52;
const int liftCLKPin = 53;

long timeOfLastDebounce = 0;
int delayOfDebounce = 0.01;

int liftPreviousCLK;
int liftPreviousDATA;

int liftCurEncValue = 0;

int armServoPin = 0;
int climbServoPin = 1;

int armAngle = 60;
int armAngleOffset = 1;


void setup() {
    bluetooth.begin(9600);

    pwm.begin();
    pwm.setPWMFreq(60);
    pwm.setPWM(armServoPin, 0, SERVOMIN);

    delay(1000);
    //arm.attach(45);
    liftPreviousCLK = digitalRead(liftCLKPin);
    liftPreviousDATA = digitalRead(liftDTPin);

}

void loop() {
    while (bluetooth.available() > 0) {
        if ((bluetooth.read()) == 'z') {
            leftXAxis = bluetooth.parseFloat();
            leftYAxis = bluetooth.parseFloat();
            trigger = bluetooth.parseFloat();
            rightYAxis = bluetooth.parseFloat();
            rightXAxis = bluetooth.parseFloat();
            aButton = bluetooth.parseInt();
            bButton = bluetooth.parseInt();
            xButton = bluetooth.parseInt();
            yButton = bluetooth.parseInt();
            leftBumper = bluetooth.parseInt();
            rightBumper = bluetooth.parseInt();
            auto1 = bluetooth.parseInt();
            auto2 = bluetooth.parseInt();
            auto3 = bluetooth.parseInt();
            auto4 = bluetooth.parseInt();
            auto5 = bluetooth.parseInt();
            if (autoReset == 0) {
                autoReset = bluetooth.parseInt();

            }
            
            drive(leftXAxis, -leftYAxis);
            turntableControl(rightXAxis);
            liftControl(rightYAxis, aButton - yButton, leftBumper - rightBumper, bButton - xButton);
            intakeControl(trigger);
           // arm.write(90);
           autonomousModes();
        }
        if ((millis() - timeOfLastDebounce) > delayOfDebounce) {
            readLiftEncoder();
            liftPreviousCLK = digitalRead(liftCLKPin);
            liftPreviousDATA = digitalRead(liftDTPin);
            timeOfLastDebounce = millis();
        }

    }
}

int readLiftEncoder() {
    if ((liftPreviousCLK == 0) && (liftPreviousDATA == 1)) {
        if ((digitalRead(liftCLKPin) == 1) && (digitalRead(liftDTPin) == 0)) {
            liftCurEncValue ++;
        }
        if ((digitalRead(liftCLKPin) == 1) && (digitalRead(liftDTPin) == 1)) {
            liftCurEncValue --;
        }
    }
    if ((liftPreviousCLK == 1) && (liftPreviousDATA == 0)) {
        if ((digitalRead(liftCLKPin) == 0) && (digitalRead(liftDTPin) == 1)) {
            liftCurEncValue ++;
        }
        if ((digitalRead(liftCLKPin) == 0) && (digitalRead(liftDTPin) == 0)) {
            liftCurEncValue --;
        }
    }
    if ((liftPreviousCLK == 1) && (liftPreviousDATA == 1)) {
        if ((digitalRead(liftCLKPin) == 0) && (digitalRead(liftDTPin) == 1)) {
            liftCurEncValue ++;
        }
        if ((digitalRead(liftCLKPin) == 0) && (digitalRead(liftDTPin) == 0)) {
            liftCurEncValue --;
        }
    }
    if ((liftPreviousCLK == 0) && (liftPreviousDATA == 0)) {
        if ((digitalRead(liftCLKPin) == 1) && (digitalRead(liftDTPin) == 0)) {
            liftCurEncValue ++;
        }
        if ((digitalRead(liftCLKPin) == 1) && (digitalRead(liftDTPin) == 1)) {
            liftCurEncValue --;
        }
    }
    return liftCurEncValue;
}

void autonomousModes() {
    //automodes
            if (auto1 == 1 && autoReset == 1) { //left cargo ship score, level 1
                drive(0, 1);
                turntableControl(1);
                //pwm.setPWM(armServoPin, 0, SERVOMIN);
                liftControl(-1, -1, 0, 0);
                delay(1000);
                liftControl(0, -1, 0, 0);
                drive(0, -1);
                delay(1000);
                drive(0, 1);
                delay(1500);
                drive(0.25, 0.7);
                delay(500);
                drive(-0.25, 0.7);
                delay(500);
                drive(0, 0);
                turntableControl(0);
                intakeControl(0);
                delay(2000);
                intakeControl(0);
                auto1 = 0;
                autoReset = 0;
            } else if (auto2 == 1 && autoReset == 1) { //right cargo ship score, level 1
                drive(0, 1);
                turntableControl(-1);
                pwm.setPWM(armServoPin, 0, SERVOMIN);
                delay(1000);
                drive(0, 0);
                turntableControl(0);
                intakeControl(0);
                delay(2000);
                intakeControl(0);
                auto2 = 0;
                autoReset = 0;
            } else if (auto3 == 1 && autoReset == 1) { //left level 2 start
                drive(0, 1);
                delay(1500);
                drive(-0.5, 1);
                delay(500);
                drive(0.5, 1);
                delay(500);
                drive(0, 0);
                auto3 = 0;
                autoReset = 0;
            } else if (auto4 == 1 && autoReset == 1) { //right level 2 start
                drive(0, 1);
                delay(1500);
                drive(-0.5, 1);
                delay(500);
                drive(0.5, 1);
                delay(500);
                drive(0, 0);
                auto4 = 0;
                autoReset = 0;
            } else if (auto5 == 1 && autoReset == 1) {
                drive(0, 1);
                liftControl(1, 0, 0, -1);
                delay(1000);
                drive(0, 0);
                for (int i = 0 ; i < 4 ; i ++) {
                    liftControl(0, 0, 1, 0);
                }
                drive(0, -1);
                delay(500);
                drive(0, 0);
            }
}
void drive(float xPower, float yPower) {
    xPower *= 100;
    yPower *= -100;
    float v = (100 - abs(yPower)) * (xPower/100) + xPower;
    float w = (100 - abs(xPower)) * (yPower/100) + yPower;
    float velocityL = ((((v - w) / 2) / 100) * 255);
    float velocityR = ((((v + w) / 2) / 100) * 255);

    dmLeft.run((velocityL >= 0) ? FORWARD : BACKWARD);
    dmLeft.setSpeed(abs(velocityL));
    dmRight.run((velocityR >= 0) ? FORWARD : BACKWARD);
    dmRight.setSpeed(abs(velocityR));
}

void turntableControl(float rotationPower) {
    if (rotationPower > 0) {
        digitalWrite(41, LOW);
        analogWrite(40, rotationPower * 240);
    } else {
        digitalWrite(40, LOW);
        analogWrite(41, -rotationPower * 240);
    }
}

void liftControl(int liftPower, int angleAdjustment, int newOffset, int mediumPosition) {
    //lift
    liftMotor.run((liftPower >= 0) ? FORWARD : BACKWARD);
    liftMotor.setSpeed(abs(liftPower) * 255);
    //arm
    // uint16_t servoPulse = ((angle/180)*(SERVOMAX-SERVOMIN))+SERVOMIN;
    // if (servoPulse > SERVOMAX) servoPulse = SERVOMAX;
    // if (servoPulse < SERVOMIN) servoPulse = SERVOMIN;
    if (angleAdjustment == 1) {
        armAngle = SERVOMAX;
        armAngleOffset = 0;
    } else if (angleAdjustment == -1) {
        armAngle = SERVOMIN;
        armAngleOffset = 0;
    } else if (mediumPosition == 1) {
        armAngle = 370;
        armAngleOffset = 0;
    } else if (mediumPosition == -1) {
        armAngle = 450;
        armAngleOffset = 0;
    }
    if (newOffset == 1) {
        armAngleOffset += 10;
    } else if (newOffset == -1) {
        armAngleOffset -= 10;
    }
    pwm.setPWM(armServoPin, 0, armAngle + armAngleOffset);
    //servoPulse += 5 * angleAdjustment;
    //pwm.setPWM(0, 0, servoPulse + servoPulseOffset);
    // if (liftCurEncValue > 100) {
    //     turntableControl(-1);
    // } else {
    //     turntableControl(1);
    // }
}

void armServoControl(int angle) {
    if (angle < 0) angle = 0;
    if (angle > 110) angle = 110;
    float servoPulse = ((angle / 110) * (SERVOMAX - SERVOMIN)) + SERVOMIN;
    pwm.setPWM(0, 0, servoPulse);
}

void intakeControl(int intakePower) {
    intakeMotor.run((intakePower >= 0) ? FORWARD : BACKWARD);
    if (abs(intakePower) > 0.8) {
        intakeMotor.setSpeed(255);
    } else {
        intakeMotor.setSpeed(0);
    }
}


// int liftEncoder() {
//     n = digitalRead(encoderPinA);
//     if ((encoder0PinALast == LOW) && (n == HIGH)) {
//         if (digitalRead(encoderPinB) == LOW) {
//             encoder0Pos--;

//         } else {
//             encoder0Pos++;
//         }
//     }
//     encoder0PinALast = n;
//     return encoder0Pos;
// }

