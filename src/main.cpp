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

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void drive() {
  // may work, else just change directions and rotation angle
  if(std::abs(leftDriveAxis().position()) > 0 || std::abs(rightDriveAxis().position()) > 0) {
    Comp_Vector driveVec(leftDriveAxis().position(), rightDriveAxis().position());
    driveVec.rotate(M_PI/4.0);
    double x = map(driveVec.get_x(), 0, cos(driveVec.get_angle()), 0, 1);
    double y = map(driveVec.get_y(), 0, sin(driveVec.get_angle()), 0, 1);
    if(x > 0) {
      FrontLeft.setVelocity(std::abs(x)*100.0, percent);
      BackRight.setVelocity(std::abs(x)*100.0, percent);
      FrontLeft.spin(forward);
      BackRight.spin(forward);
    } else {
      FrontLeft.setVelocity(std::abs(x)*100.0, percent);
      BackRight.setVelocity(std::abs(x)*100.0, percent);
      FrontLeft.spin(reverse);
      BackRight.spin(reverse);
    }
    if(y > 0) {
      FrontRight.setVelocity(std::abs(x)*100.0, percent);
      BackLeft.setVelocity(std::abs(x)*100.0, percent);
      FrontRight.spin(forward);
      BackLeft.spin(forward);
    } else {
      FrontRight.setVelocity(std::abs(x)*100.0, percent);
      BackLeft.setVelocity(std::abs(x)*100.0, percent);
      FrontRight.spin(reverse);
      BackLeft.spin(reverse);
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
  while(true) drive();
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

void run() {
}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  init();

  teleop();
}