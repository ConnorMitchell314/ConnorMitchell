#include <Servo.h>
#include <time.h>
#include "ds_interface.h"

#define RX_FROM_NMCU 0
#define TX_TO_NMCU 1

Servo FL;
Servo FR;
Servo BL;
Servo BR;
Servo S;
Servo Claw;
DS_Interface ds(RX_FROM_NMCU, TX_TO_NMCU);


int pinServoFL = 6;
int pinServoFR = 5;
int pinServoBL = 11;
int pinServoBR = 10;
int pinServoS = 2;
int pinServoClaw = 3;

 void setup() {
     FL.attach(6);
     FR.attach(5);
     BL.attach(11);
     BR.attach(10);
     S.attach(2);
     Claw.attach(3);

     Serial.println("Interfacing arduino with nodemcu");
     ds.init();
 }

 void loop() {
     char input = ds.readInputIfAvailable();

     switch(input) {
        case 'r':
            forward();
            break;
        case 'f':
            back();
            break;
        case 'd':
            turnLeft();
            break;
        case 'g':
            turnRight();
            break;
        case ' ':
            stopDrive();
            break;
        case 'e':
            Claw.write(180);
            break;
        case 't':
            Claw.write(0);
            break;
        default:
           // stopDrive();
            break;
     }
 }


 void forward() {
     BL.write(0);
     BR.write(180);
     FL.write(0);
     FR.write(180);
 }

 void back() {
     BL.write(180);
     BR.write(0);
     FL.write(180);
     FR.write(0);
 }

 void left() {
     S.write(180);
 }

 void right() {
     S.write(0);
 }

 void stopAll() {
     BL.write(90);
     BR.write(90);
     FL.write(90);
     FR.write(90);
    // S.write(90);
 }

 void stopStrafe() {
    // S.write(90);
 }

 void stopDrive() {
     BL.write(90);
     BR.write(90);
     FL.write(90);
     FR.write(90);
 }

 void turnRight() {
     BL.write(0);
     BR.write(0);
     FL.write(0);
     FR.write(0);
     //S.write(90);
 }

 void turnLeft() {
     BL.write(180);
     BR.write(180);
     FL.write(180);
     FR.write(180);
    // S.write(90);
 }
