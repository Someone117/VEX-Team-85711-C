// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// FrontLeft            motor         9
// FrontRight           motor         3
// BackLeft             motor         1
// BackRight            motor         2
// Indexer              motor         18
// Intake               motor         16
// Flywheel             motor         10
// EndGame              motor         8
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
#include "vex.h"
#include "xDrive.h"

using namespace vex;
using namespace std;

competition Competition;

controller::axis upDownAxis() { return Controller1.Axis3; }
controller::axis leftRightAxis() { return Controller1.Axis4; }
controller::axis turnAxis() { return Controller1.Axis1; }

int indexerTasks() {
  bool ret = false; // does the endgame need to return, or trigger
  while (true) {
    if (Controller1.ButtonR1.pressing() &&
        !Indexer.isSpinning()) { // If the indexer finished, start it spinning
      Indexer.startSpinFor(vex::reverse, 360, deg);
    }
    if (Controller1.ButtonDown.pressing()) {
      if (!EndGame.isSpinning()) {
        if (ret) {
          EndGame.startSpinFor(vex::reverse, 130, degrees);
          ret = false;
        } else {
          EndGame.startSpinFor(vex::forward, 130, degrees);
          ret = true;
        }
      }
    }
    this_thread::sleep_for(50);
  }
  return 1;
}

int flywheelTasks() {
  bool fly = true;                // should flywheel be spinning
  bool flyWheelSpinState = true; // which velocity should it be spinning at
  while (true) {
    if (Controller1.ButtonUp.pressing()) {
      if (fly) {
        Flywheel.stop();
        fly = false;
      } else {
        Flywheel.spin(vex::forward);
        fly = true;
      }
    } else if (Controller1.ButtonA.pressing()) {
      if (flyWheelSpinState) {
        Flywheel.setVelocity(475, rpm);
      } else {
        Flywheel.setVelocity(415, rpm); // default
      }
    }
    this_thread::sleep_for(100);
  }
  return 1;
}

void run(bool flipDrive) { // drive and intake
  double deadZone = 15;    // prevents drifting
  Comp_Vector driveVec(
      abs(upDownAxis().position()) > deadZone ? upDownAxis().position() : 0,
      abs(leftRightAxis().position()) > deadZone ? -leftRightAxis().position() : 0); // snap to 0 if any individual axis is in the deadzone
  double turnAmt = turnAxis().position();
  if (std::abs(turnAmt) > deadZone ||
      driveVec.get_mag() > deadZone) { // are we outside the deadzone
    if (driveVec.get_mag() > std::abs(turnAmt)) {
      drive(driveVec.get_mag() > deadZone ? driveVec : Comp_Vector(0, 0),
            flipDrive);
    } else {
      turn(std::abs(turnAmt) > deadZone ? turnAmt : 0);
    }
  } else {
    stopDrive();
  }

  if (Controller1.ButtonL2.pressing()) { // spin intake in
    Intake.setVelocity(100, percent);
    Intake.spin(vex::forward);
  } else if (Controller1.ButtonR2.pressing()) { // spin intake out
    Intake.setVelocity(100, percent);
    Intake.spin(vex::reverse);
  } else if (Controller1.ButtonL2.pressing()) { // spin rollers
    Intake.setVelocity(70, percent);
    Intake.spin(vex::reverse);
  } else {
    Intake.stop();
  }
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

  Flywheel.setStopping(coast);
  Flywheel.setVelocity(415, rpm);

  Indexer.setStopping(brake);
  Indexer.setVelocity(50, percent);

  Intake.setStopping(brake);

  EndGame.setVelocity(100, percent);

  stopDrive(); // safety
}

void teleop() {
  init();
  Flywheel.spin(vex::forward); // start flywheel
  stopDrive();
  thread indexer_thread(indexerTasks); // enable the indexer
  bool flip = false;                   // should we reverse the drive
  while (true) {
    run(flip);
    if (Controller1.ButtonX.pressing()) {
      flip = !flip;
    }
  }
  thread::interruptAll();
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  init();

  // Skills test
  // Controller1.ButtonA.pressed(auton);

  Competition.autonomous([]() { auton(L); });
  Competition.drivercontrol(teleop);
}