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

controller::axis upDownAxis() { return Controller1.Axis3; }
controller::axis leftRightAxis() { return Controller1.Axis4; }
controller::axis turnAxis() { return Controller1.Axis1; }

void index_forward(){
  Indexer.spin(vex::forward);
  this_thread::sleep_for(250);
  Indexer.stop();
  this_thread::sleep_for(200);
}

void index_backward(){
  Indexer.spin(reverse);
  this_thread::sleep_for(250);
  Indexer.stop();
}

// we may want to make it not get stuck accedentally
int indexerTasks() {
  bool ret = false;
  bool ret1 = false;
  while(true) {
    if(Controller1.ButtonR1.pressing() && !ret) {
      index_backward();
      ret = true;
    } else if(ret) {
      index_forward(); 
      ret = false;
    }
    if(Controller1.ButtonDown.pressing()) {
      if(ret1) {
        //TODO: CKECK AND MAKE BETTER ENDGAME
        EndGame.spinFor(vex::reverse, 130, degrees);
        ret1 = false;
      } else {
        EndGame.spinFor(vex::forward, 130, degrees);
        ret1 = true;
      }
    }
    this_thread::sleep_for(50);
  }
  return 1;
}

int flywheelTasks() {
  bool fly = true;
  bool fly1 = false;
  while(true) {
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
    } else if(Controller1.ButtonA.pressing()) {
      if(fly1) {
        Flywheel.setVelocity(475, rpm);
      } else {
        Flywheel.setVelocity(415, rpm);
      }
      
    }
  }
  return 1;
}

void run(bool flipDrive) {
  double deadZone = 15;
  Comp_Vector driveVec(abs(upDownAxis().position()) > deadZone ? upDownAxis().position() : 0, abs(leftRightAxis().position()) > deadZone ? -leftRightAxis().position() : 0);
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

  EndGame.setVelocity(100, percent);


  stopDrive();
}

void teleop() {
  init();
  Flywheel.spin(vex::forward);
  stopDrive();
  thread indexer_thread(indexerTasks);
  bool flip = false;
  while(true) {
    run(flip);
    if(Controller1.ButtonX.pressing()) {
      flip= !flip;
    }
  }
  thread::interruptAll();
}

enum autons {
  L, 
  R,
  SKILLS,
  DISABLED
};

void auton_skills() {

}

void auton_left() {
  Flywheel.spin(vex::forward); 

  //Drives into and rolls the close roller
  Comp_Vector a(-50, 0);
  drive(a, false);
  this_thread::sleep_for(500);
  stopDrive();
  Intake.spin(vex::reverse, 70, percent);
  this_thread::sleep_for(250);
  Intake.stop(); 

  //gets off the roller to position and shoots
  a.set_mag(a.get_mag()*-1);
  drive(a, false);
  this_thread::sleep_for(500);
  for(int i = 0; i < 2; i++){
    index_forward();
    index_backward();
  }
  Flywheel.stop();
  
  // should uno reverse and drive into the other roller backwards
  turn(-50);
  this_thread::sleep_for(500);
  drive(a, false);
  this_thread::sleep_for(750);
  stopDrive();
  Intake.spin(vex::reverse, 70, percent);
  this_thread::sleep_for(250);
  Intake.stop(); 
}

void auton_right() {
  Flywheel.spin(vex::forward); 

  // drive left
  Comp_Vector a(-50, 0);
  Comp_Vector b(0, -50);
  drive(b, false);
  this_thread::sleep_for(500);
  stopDrive();
  // drive backwards and spin roller
  drive(a, false);
  this_thread::sleep_for(500);
  stopDrive();
  Intake.spin(vex::reverse, 70, percent);
  this_thread::sleep_for(250);
  Intake.stop();
  // shoot twice
  for(int i = 0; i < 2; i++){
    index_forward();
    index_backward();
  }
  Flywheel.stop();
}

void auton(autons aut) {
  switch(aut) {
    case L: auton_left();
    case R: auton_right();
    case SKILLS: auton_skills();
    case DISABLED:;
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  init();

  // Skills test
  //Controller1.ButtonA.pressed(auton);

  Competition.autonomous([](){ auton(L); });
  Competition.drivercontrol(teleop);
}