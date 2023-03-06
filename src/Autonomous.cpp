#include "autonomous.h"
#include "tankDrive.h"
#include "vex.h"
#include "robot-config.h"
#include "Main_PID.h"

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


double volts_checked(double volts, double max){
  if(volts > max){
    return max;
  }
  if(volts < -max){
    return -max;
  }
  return volts; 
}


// this function finds a similar speed for the motorgroups
void auto_drive(double volts, double MAX_V, double c){
  double diff = std::abs(LeftDrive.rotation(rotationUnits::rev)) - std::abs(RightDrive.rotation(rotationUnits::rev));
  int sign = diff < 0 ? -1 : 1; 
  double dV = sign*(diff)*c; // c is a constant that needs tuning

  LeftDrive.spin(forward, volts, voltageUnits::volt);
  RightDrive.spin(forward, volts_checked(volts + dV, MAX_V), voltageUnits::volt);
}

void auto_drive_dist(int dist){
  // reseting the rotation vals to 0
  LeftDrive.resetRotation();
  RightDrive.resetRotation();
  const double wheel_diameter = 3.25;
  const double MAX_V = 5;

  double total_revs = dist/(wheel_diameter*M_PI); // converts inches into revolutions
  double gear_ratio = 2.0/3; 
  double volts;

  // PID
  Main_PID pid(total_revs); 
  while(LeftDrive.rotation(rotationUnits::rev)*gear_ratio < total_revs){
    pid.set_e_cur(total_revs - std::abs(LeftDrive.rotation(rotationUnits::rev)*gear_ratio) ); // total_revs - cur_revs
    pid.sum_int();
    pid.find_deriv();
    volts = volts_checked(pid.output(), MAX_V);
    auto_drive(volts, MAX_V, 0.05);

    this_thread::sleep_for(50);
  }
 
  auto_drive(0, MAX_V, 1); 
}

void auto_turn(double volts){
  LeftDrive.spin(forward, volts, voltageUnits::volt);
  RightDrive.spin(forward, -volts, voltageUnits::volt);
}

void auto_turn_deg(double volts, int deg){
  double r = 0.2; //this needs to be tuned
  volts = volts_checked(volts, 5); 

  LeftDrive.resetRotation();
  RightDrive.resetRotation();

  //slows down the bot as it approaches it makes rev
  //also gear_ratio doesn't matter because r accounts for it I think
  while(std::abs(LeftDrive.rotation(rotationUnits::rev))/r < deg*0.8){
    auto_turn(volts);
  }
  while(std::abs(LeftDrive.rotation(rotationUnits::rev))/r < deg*0.8){
    auto_turn(volts/3);
  }
  
  auto_turn(-volts/2);
  this_thread::sleep_for(50);
  auto_turn(0); 
}

/*
To use this function, enter the speed, in rpms, and a r_test.
Decide on a degree amount you want the bot to turn: maybe 90 degrees.
Keep changing r_test until the bot actually reaches that 90 degrees. 
*/
void tune_r_for_turn(double volts, int r_test){
  volts = volts_checked(volts, 5); 
  LeftDrive.resetRotation();
  RightDrive.resetRotation();

  while(std::abs(LeftDrive.rotation(rotationUnits::rev)) < r_test){
    auto_turn(volts);
  }

  auto_turn(-volts/2);
  this_thread::sleep_for(50);
  auto_turn(0); 
}



void auton(autons aut) { // choose which auton
  switch (aut) {
  case L:
    auton_left();
    return;
  case R:
    auton_right();
    return;
  case SKILLS:
    auton_skills();
    return;
  case L_SIMPLE:
    auton_left_simple();
    return;
  case SKILLS_2:
    auton_skills_2();
    return;
  case DISABLED:return;;
  }
}