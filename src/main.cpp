/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Someo                                            */
/*    Created:      Tue Aug 30 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Motor7               motor         7               
// LeftMotors           motor_group   1, 2            
// RightMotors          motor_group   3, 4            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "drive.h"

using namespace vex;

controller::axis leftDriveAxis() { return Controller1.Axis3; }
controller::axis rightDriveAxis() { return Controller1.Axis2; }
controller::button fly() { return Controller1.ButtonA; }

void drive() {
  leftDriveAxis().changed([](){ powerLeft(leftDriveAxis().position()); });
  rightDriveAxis().changed([](){ powerRight(rightDriveAxis().position()); });

  if(fly().pressing()) {
    Motor7.spin(forward);
  } else {
    Motor7.stop();
  }

  wait(100, msec);
}

void teleop() {
  while(true) drive();
}

void init() {
  Motor7.setMaxTorque(100, percent);
  Motor7.setStopping(coast);
  
}

void run() {
}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  teleop();
}
