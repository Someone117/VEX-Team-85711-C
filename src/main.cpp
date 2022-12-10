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
// FrontLeft            motor         5               
// FrontRight           motor         19              
// BackLeft             motor         2               
// BackRight            motor         13              
// Indexer              motor         1               
// Intake               motor         6               
// Flywheel             motor         16              
// EndGame              motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "vex.h"
#include "xDrive.h"
#include "Autonomous.h"

using namespace vex;

competition Competition;

controller::axis leftDriveAxis() { return Controller1.Axis3; }
controller::axis rightDriveAxis() { return Controller1.Axis4; }
controller::axis turnAxis() { return Controller1.Axis1; }


// we may want to make it not get stuck accedentally
int indexerTasks() {
  bool ret = false;
  while(true) {
    // Indexer.setVelocity(80, percent);
    // if(Controller1.ButtonR1.pressing()) {
    //   Indexer.spin(reverse);
    //   this_thread::sleep_for(150);
    //   Indexer.stop();
    // } else if(Controller1.ButtonR2.pressing()) {
    //   Indexer.spin(vex::forward);
    //   this_thread::sleep_for(150);
    //   Indexer.stop();
    // }
    
    if(Controller1.ButtonR1.pressing() && !ret) {
      Indexer.spin(reverse);
      this_thread::sleep_for(250);
      Indexer.stop();
      ret = true;
      this_thread::sleep_for(200);
    } else if(ret) {
      Indexer.spin(vex::forward);
      this_thread::sleep_for(250);
      Indexer.stop();
      ret = false;
    }
    if(Controller1.ButtonDown.pressing()) {
      EndGame.setVelocity(100, percent);
      EndGame.spinFor(130, degrees);
    }

    this_thread::sleep_for(50);
  }
  return 1;
}

void run(bool flipDrive) {
  double deadZone = 15;
  Comp_Vector driveVec(abs(leftDriveAxis().position()) > deadZone ? leftDriveAxis().position() : 0, abs(rightDriveAxis().position()) > deadZone ? -rightDriveAxis().position() : 0);
  double turnAmt = turnAxis().position();
  if(abs(turnAmt) > deadZone || driveVec.get_mag() > deadZone) {
    if(driveVec.get_mag() > abs(turnAmt)) {
      drive(driveVec.get_mag() > deadZone ? driveVec : Comp_Vector(0,0), flipDrive);
    } else {
      turn(abs(turnAmt) > deadZone ? turnAmt : 0);
    }
  } else {
    stopDrive();
  }

  if(Controller1.ButtonL2.pressing()) {
    //Intake.setVelocity(100, percent);
    //Intake.spin(vex::forward);
  } else if(Controller1.ButtonR2.pressing()) {
    //Intake.setVelocity(100, percent);
    //Intake.spin(vex::reverse);
  } //else 
  if(Controller1.ButtonL2.pressing()) {
    Intake.setVelocity(70, percent);
    Intake.spin(vex::reverse);
  } else {
    Intake.stop();
  }
  
  this_thread::sleep_for(50);
}

void init() {
  FrontLeft.setMaxTorque(100,percent);
  FrontLeft.setStopping(brake);
  FrontRight.setMaxTorque(100,percent);
  FrontRight.setStopping(brake);
  BackLeft.setMaxTorque(100,percent);
  BackLeft.setStopping(brake);
  BackRight.setMaxTorque(100,percent);
  BackRight.setStopping(brake);
  
  Flywheel.setStopping(coast);
  Flywheel.setVelocity(415,rpm);
  
  Indexer.setStopping(brake);
  Indexer.setVelocity(50,percent);
  
  Intake.setStopping(brake);

  stopDrive();
}

void teleop() {
  init();
  Flywheel.spin(vex::forward);
  stopDrive();
  thread indexer_thread(indexerTasks);
  bool flip = false;
  bool fly = true;
  while(true) {
    run(flip);
    if(Controller1.ButtonX.pressing()) {
      flip= !flip;
      this_thread::sleep_for(100);
    }
    if(Controller1.ButtonUp.pressing()) {
    if(fly) {
      Flywheel.stop();
      fly = false;
      this_thread::sleep_for(100);

    } else {
      Flywheel.spin(vex::forward);
      fly = true;
      this_thread::sleep_for(100);
    }
  }
  }
  thread::interruptAll();
}

enum autons {
  BLUE_L,
  BLUE_R,
  RED_L,
  RED_R,
  DISABLED
};

void auton(autons aut) {
  if(aut == DISABLED) return;
  Comp_Vector a(-50, 0);
  drive(a, false);
  this_thread::sleep_for(500);
  stopDrive();
  Intake.spin(aut == BLUE_L ? vex::reverse : vex::forward, 70, percent);
  this_thread::sleep_for(250);
  Intake.stop();
}

void auton_Skills() {

}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  init();

  // Skills test
  //Controller1.ButtonA.pressed(auton);

  Competition.autonomous([](){ auton(BLUE_L); });
  Competition.drivercontrol(teleop);
}