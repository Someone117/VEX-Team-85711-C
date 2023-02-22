#include "autonomous.h"
#include "tankDrive.h"
#include "vex.h"


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