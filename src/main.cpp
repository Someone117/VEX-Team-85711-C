/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Vex Team 8511C                                            */
/*    Created:      Tue Aug 30 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// FrontLeft            motor         3               
// FrontRight           motor         10              
// BackLeft             motor         11              
// BackRight            motor         15              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "xDrive.h"

using namespace vex;

controller::axis leftDriveAxis() { return Controller1.Axis3; }
controller::axis rightDriveAxis() { return Controller1.Axis4; }
controller::axis turnAxis() { return Controller1.Axis1; }

void run() {
  Comp_Vector driveVec(leftDriveAxis().position(), rightDriveAxis().position());
  double deadZone = 10;

  if(abs(turnAxis().position()) > deadZone || driveVec.get_mag() > deadZone) {
    if(driveVec.get_mag() > abs(turnAxis().position())) {
      drive(driveVec);
    } else {
      turn(turnAxis().position());
    }
  } else {
    FrontLeft.stop();
    FrontRight.stop();
    BackLeft.stop();
    BackRight.stop();
  }
  wait(100, msec);
}

void teleop() {
  while(true) run();
}

void init() {
  //Drive motors
  // should we set velocity??
  FrontLeft.setMaxTorque(100,percent);
  FrontLeft.setStopping(coast);
  FrontRight.setMaxTorque(100,percent);
  FrontRight.setStopping(coast);
  BackLeft.setMaxTorque(100,percent);
  BackLeft.setStopping(coast);
  BackRight.setMaxTorque(100,percent);
  BackRight.setStopping(coast);
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  init();

  teleop();
}