//#include <Wire.h>
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

//Driver Station inputs
float leftXAxis;
float leftYAxis;
float trigger;
float rightYAxis;
float rightXAxis;

#define SERVOMIN 150
#define SERVOMAX 550

int angle = 90;

int turretEncoderPinA = 0;
int turretEncoderPinB = 0;
int liftEncoderPinA = 0;
int liftEncoderPinB = 0;

int armServoPin = 0;
int climbServoPin = 1;

void setup() {
    bluetooth.begin(9600);

    pwm.begin();
    pwm.setPWMFreq(60);
    
    pinMode(turretEncoderPinA, INPUT);
    pinMode(turretEncoderPinB, INPUT);
    pinMode(liftEncoderPinA, INPUT);
    pinMode(liftEncoderPinB, INPUT);

    delay(1000);
}

void loop() {
    while (bluetooth.available() > 0) {
        if ((bluetooth.read()) == 'z') {
            leftXAxis = bluetooth.parseFloat() * 100;
            leftYAxis = bluetooth.parseFloat() * 100;
            trigger = bluetooth.parseFloat();
            rightYAxis = bluetooth.parseFloat() * 255;
            rightXAxis = bluetooth.parseFloat();
            drive(leftXAxis, leftYAxis);
            turntableControl(rightXAxis);
            liftControl(rightYAxis);
            intakeControl(trigger);
        } 
    }
}

void drive(int xPower, int yPower) {
    float v = (100 - abs(yPower)) * (xPower/100) + xPower;
    float w = (100 - abs(xPower)) * (yPower/100) + yPower;
    float velocityL = ((((v - w) / 2) / 100) * 255);
    float velocityR = ((((v + w) / 2) / 100) * 255);

    dmLeft.run((velocityL >= 0) ? FORWARD : BACKWARD);
    dmLeft.setSpeed(abs(velocityL));
    dmRight.run((velocityR >= 0) ? FORWARD : BACKWARD);
    dmRight.setSpeed(abs(velocityR));
}

void turntableControl(int rotationPower) {
    if (rotationPower > 0) {
        digitalWrite(40, LOW);
        analogWrite(41, rotationPower * 240);
    } else {
        digitalWrite(41, LOW);
        analogWrite(40, -rotationPower * 240);
    }
}

void liftControl(int liftPower) {
    //lift
    liftMotor.run((liftPower >= 0) ? FORWARD : BACKWARD);
    liftMotor.setSpeed(abs(liftPower));
    //arm
    uint16_t servoPulse = ((angle/180)*(SERVOMAX-SERVOMIN))+SERVOMIN;
    if (servoPulse > SERVOMAX) servoPulse = SERVOMAX;
    if (servoPulse < SERVOMIN) servoPulse = SERVOMIN;
    pwm.setPWM(0, 0, 300);
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

