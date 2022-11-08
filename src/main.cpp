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
// Flywheel             motor         13              
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "vex.h"
#include "xDrive.h"
#include "Autonomous.h"

using namespace vex;

competition Competition;

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
  //wait(100, msec);
  task::sleep(100);
}

void teleop() {
  void init();
  while(true) run();
}


void auton() {
  FlywheelCommand a(true, nullptr, false);
  a.execute();
  task::sleep(3000);
  Flywheel.stop();
}

void auton_Skills() {

}

void init() {
  FrontLeft.setMaxTorque(100,percent);
  FrontLeft.setStopping(coast);
  FrontRight.setMaxTorque(100,percent);
  FrontRight.setStopping(coast);
  BackLeft.setMaxTorque(100,percent);
  BackLeft.setStopping(coast);
  BackRight.setMaxTorque(100,percent);
  BackRight.setStopping(coast);
  Flywheel.setStopping(coast);
  Flywheel.setVelocity(100,percent);
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  init();

  // Skills test
  Controller1.ButtonA.pressed(auton);

  Competition.drivercontrol(teleop);
  Competition.autonomous([](){ auton(); });
}