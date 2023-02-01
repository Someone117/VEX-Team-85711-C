// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// FrontLeft            motor         6
// FrontRight           motor         3
// BackLeft             motor         1
// BackRight            motor         2
// Intake               motor         16
// Cata                 motor         8
// TopLeft              motor         4
// TopRight             motor         5
// ---- END VEXCODE CONFIGURED DEVICES ----

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Vex Team 8511C                                            */
/*    Created:      Tue Aug 30 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "autonomous.h"
#include "tankDrive.h"
#include "vex.h"


using namespace vex;
using namespace std;

competition Competition;

controller::axis driveAxis() { return Controller1.Axis3; }
controller::axis turnAxis() { return Controller1.Axis1; }

void run(bool flipDrive) { // does everything
  // Driving
  double deadZone = 15; // prevents drifting
  double driveAmt = driveAxis().position();
  // turning
  double turnAmt = turnAxis().position();
  if (std::abs(driveAmt) > std::abs(turnAmt)) {
    if(abs(driveAmt) > deadZone) {
      drive(driveAmt, flipDrive);
    } else {
      stopDrive();
    }
  } else {
    if(abs(turnAmt) > deadZone) {
      turn(turnAmt, flipDrive);
    } else {
      stopDrive();
    }
  }

  // Intake and Rollers
  if (Controller1.ButtonL2.pressing()) { // spin intake in
    Intake.setVelocity(100, percent);
    Intake.spin(vex::forward);
  } else if (Controller1.ButtonR2.pressing()) { // spin intake out
    Intake.setVelocity(100, percent);
    Intake.spin(vex::reverse);
  } else {
    Intake.stop();
  }

  //Cata
  this_thread::sleep_for(50);
}

void init() { // init all torque, velocity and breaking
  FrontLeft.setMaxTorque(100, percent);
  FrontLeft.setStopping(brake);
  FrontRight.setMaxTorque(100, percent);
  FrontRight.setStopping(brake);
  BackLeft.setMaxTorque(100, percent);
  BackLeft.setStopping(brake);
  BackRight.setMaxTorque(100, percent);
  BackRight.setStopping(brake);
  TopRight.setMaxTorque(100, percent);
  TopLeft.setMaxTorque(100, percent);
  TopLeft.setStopping(brake);
  TopRight.setStopping(brake);
  Intake.setStopping(brake);

  Cata.setVelocity(100, percent);
  Cata.setStopping(hold);

  stopDrive(); // safety
}

void teleop() {
  init();
  // need to start cata
  stopDrive();
  bool flip = false; // should we reverse the drive
  // bool endGameReturn = false; // did we activate the endgame?
  while (true) {
    // Drive, Flywheel, intake and indexer
    run(flip);

    // Drive inversion
    if (Controller1.ButtonB.pressing()) {
      flip = !flip;
      this_thread::sleep_for(100);
    }
    if (flip) {
      Controller1.rumble("-");
    }
    // Endgame
    // if (Controller1.ButtonY.pressing()) {
    //   if (endGameReturn) {
    //     EndGame.startSpinFor(vex::reverse, 130, degrees);
    //     endGameReturn = false;
    //   }
    // }
  }
  thread::interruptAll();
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  init();

  // Skills test
  // Controller1.ButtonA.pressed(auton);

  Competition.autonomous([]() { auton(L_SIMPLE); }); // Which auton to do
  Competition.drivercontrol(teleop);
}