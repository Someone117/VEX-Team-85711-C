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
// FrontLeft            motor         3               
// FrontRight           motor         10              
// BackLeft             motor         11              
// BackRight            motor         15              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "Math_Vector.h"

using namespace vex;

controller::axis leftDriveAxis() { return Controller1.Axis3; }
controller::axis rightDriveAxis() { return Controller1.Axis4; }
controller::axis turnAxis() { return Controller1.Axis1; }

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//TODO: Check spining
void turn(double v) {
  bool FL = v > 0;
  bool FR = v > 0;
  bool BL = v < 0;
  bool BR = v < 0;

  FrontLeft.setVelocity(v, percent);
  FrontRight.setVelocity(v, percent);
  BackLeft.setVelocity(v, percent);
  BackRight.setVelocity(v, percent);
  
  FrontLeft.spin(FL ? forward : reverse);
  FrontRight.spin(FR ? forward : reverse);
  BackLeft.spin(BL ? forward : reverse);
  BackRight.spin(BR ? forward : reverse);
}

//TODO: Check drive
void drive(Comp_Vector driveVec) {
  driveVec.rotate(-M_PI/4.0);
  
  bool FL = 0;
  bool FR = 0;
  bool BL = 0;
  bool BR = 0;
  double x = map(std::abs(driveVec.get_x()), 0, std::abs(100*cos(driveVec.get_angle())), 0, 100);
  double y = map(std::abs(driveVec.get_y()), 0, std::abs(100*sin(driveVec.get_angle())), 0, 100);

  FL = driveVec.get_x() > 0;
  FR = driveVec.get_y() > 0;
  BL = driveVec.get_y() < 0;
  BR = driveVec.get_x() < 0;

  FrontLeft.setVelocity(x, percent);
  FrontRight.setVelocity(y, percent);
  BackLeft.setVelocity(x, percent);
  BackRight.setVelocity(y, percent);

  FrontLeft.spin(FL ? forward : reverse);
  FrontRight.spin(FR ? forward : reverse);
  BackLeft.spin(BL ? forward : reverse);
  BackRight.spin(BR ? forward : reverse);
}

void run() {
  // I can make it so that you can drive and spin but that is pointless
  Comp_Vector driveVec(leftDriveAxis().position(), rightDriveAxis().position());
  if(driveVec.get_mag() > 5) {
    drive(driveVec);
  } else if(abs(turnAxis().position()) > 5) {
    turn(turnAxis().position());
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