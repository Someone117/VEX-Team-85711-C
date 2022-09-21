#include "xDrive.h"

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//TODO: Check spining
void turn(double v) {
  bool FL = v > 0;
  bool FR = v > 0;
  bool BL = v > 0;
  bool BR = v > 0;

  FrontLeft.setVelocity(std::abs(v), percent);
  FrontRight.setVelocity(std::abs(v), percent);
  BackLeft.setVelocity(std::abs(v), percent);
  BackRight.setVelocity(std::abs(v), percent);
  
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