#include "autonomous.h"
#include "tankDrive.h"
#include "vex.h"
#include "robot-config.h"
#include "Controllers.h"
#include "iostream"

void auton_skills() {
  drive(-75, false);
  this_thread::sleep_for(100);
  drive(-15, false);
  Intake.spin(vex::forward);
  this_thread::sleep_for(1500);
  Intake.stop();
  stopDrive();
  drive(75, false);
  this_thread::sleep_for(500);
  drive(0, false);
  turn(100);
  this_thread::sleep_for(200);
  turn(0);
  this_thread::sleep_for(100);
  drive(60, false);
  this_thread::sleep_for(600);
  drive(0, false);
}

void auton_skills_2() {
  drive(-75, false);
  this_thread::sleep_for(100);
  drive(-15, false);
  Intake.spin(vex::forward);
  this_thread::sleep_for(1500);
  Intake.stop();
  stopDrive();
  
  drive(75, false);
  this_thread::sleep_for(925);
  drive(0, false);

  turn(-100);
  this_thread::sleep_for(320);
  turn(0);
  
  this_thread::sleep_for(100);
  drive(-75, false);
  Intake.spin(vex::reverse, 100, percent);
  this_thread::sleep_for(1400);
  Intake.stop();
  this_thread::sleep_for(200);
  drive(0, false);
  
  //Intake.spin(vex::forward);
  //drive(-15, false);
  //Intake.spin(vex::forward);
  //Intake.stop();
  //this_thread::sleep_for(1500);
  //Intake.stop();
  stopDrive();

  drive(75, false);
  this_thread::sleep_for(600);
  drive(0, false);
  turn(100);
  this_thread::sleep_for(250);
  turn(0);
  this_thread::sleep_for(275);
  drive(60, false);
  this_thread::sleep_for(1300);
  drive(0, false);
}

void auton_left() {
  drive(-75, false);
  this_thread::sleep_for(200);
  drive(-15, false);
  Intake.spin(vex::forward);
  this_thread::sleep_for(750);
  Intake.stop();
  stopDrive();
  // drive(75, false);
  // this_thread::sleep_for(100);
  // drive(0, false);
}

void auton_right() {
  drive(75, false);
  this_thread::sleep_for(1000);
  drive(0, false);

  turn(100);
  this_thread::sleep_for(320);
  turn(0);
  this_thread::sleep_for(100);
  drive(-75, false);
  this_thread::sleep_for(400);
  drive(0, false);

  drive(-15, false);
  Intake.spin(vex::forward);
  this_thread::sleep_for(850);
  Intake.stop();
  stopDrive();
}

void auton_left_simple() {}


double volts_checked(double volts, double max, double min){
  if(volts > max){
    return max;
  }
  if(volts < min){
    return min;
  }
  return volts; 
}


// this function finds a similar speed for the motorgroups
void auto_drive(double volts, double MAX_V, double c){
  double diff = std::abs(LeftDrive.rotation(rotationUnits::rev)) - std::abs(RightDrive.rotation(rotationUnits::rev));
  int sign = (diff < 0) ? -1 : 1; 
  double dV = sign*(diff)*c; // c is a constant that needs tuning
  Brain.Screen.print(c);
  Brain.Screen.newLine();
  LeftDrive.spin(forward, volts, voltageUnits::volt);
  RightDrive.spin(forward, volts + dV, voltageUnits::volt);
}

void auto_drive_dist(int dist, double MAX_V, double MIN_V, double kP, double kI, double kD){
  LeftDrive.resetRotation();
  RightDrive.resetRotation();
  dist = std::abs(dist);
  const double wheel_diameter = 3.25;
  double total_revs = dist/(wheel_diameter*M_PI); // converts inches into revolutions
  double gear_ratio = 1.653; // works as a multiplier to account for undershooting
  double volts;
  double dT = 50;
  Main_PID pid(total_revs, kP, kI, kD, dT);
  while(total_revs*gear_ratio > std::abs(LeftDrive.rotation(rotationUnits::rev))){
    pid.set_e_cur(std::abs(total_revs) - std::abs(LeftDrive.rotation(rotationUnits::rev)*gear_ratio) ); // total_revs - cur_revs
    pid.sum_int();
    pid.find_deriv();
    double v = pid.output();
    volts = volts_checked(v, MAX_V, MIN_V);
    Brain.Screen.print(v); 
    Brain.Screen.newLine();
      
    auto_drive(volts, MAX_V, 0.1); //just changed from 0.05

    this_thread::sleep_for(dT);
  }
  Brain.Screen.print("Done");
  auto_drive(0, MAX_V, 0);
}

void auto_turn(double volts){
  LeftDrive.spin(forward, volts, voltageUnits::volt);
  RightDrive.spin(forward, -volts, voltageUnits::volt);
}

void auto_turn_deg_PID(int deg, double kP, double kI, double kD, double MAX_V, double MIN_V){
  // reseting the rotation vals to 0
  LeftDrive.resetRotation();
  RightDrive.resetRotation();
  deg = std::abs(deg);
  const double wheel_diameter = 3.25;
  const double r = 6;
  double dist = deg*M_PI/180*r;
  double total_revs = dist/(wheel_diameter*M_PI); // converts inches into revolutions
  double gear_ratio = 2.0/3; 
  double volts;

  // PID
  
  double dT = 50;
  Main_PID pid(total_revs, kP, kI, kD, dT); 
  while(std::abs(LeftDrive.rotation(rotationUnits::rev))*gear_ratio < total_revs){
    pid.set_e_cur(std::abs(total_revs) - std::abs(LeftDrive.rotation(rotationUnits::rev)*gear_ratio) ); // total_revs - cur_revs
    pid.sum_int();
    pid.find_deriv();
    double v = pid.output();
    volts = volts_checked(v, MAX_V, MIN_V);
    Brain.Screen.print(v); 
    Brain.Screen.newLine();
      
    auto_turn(volts);

    this_thread::sleep_for(dT);
  }
 
  auto_turn(0); 
}

void auton(config c) { // choose which auton
  switch (c) {
  case AUTO_SKILLS:
    auton_skills();
    return;
  case AUTO_SKILLS_SAFE:
    auton_skills_2();
    return;
  case DRIVER_SKILLS:
    return;
  case MATCH_L:
    auton_left();
    return;
  case MATCH_R:
    auton_right();
    return;
  case NO_AUTO:
    return;
  case TESTER:
    return;
  }
}