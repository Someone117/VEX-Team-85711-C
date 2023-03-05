#include "autonomous.h"
#include "tankDrive.h"

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

  // Intake.spin(vex::forward);
  // drive(-15, false);
  // Intake.spin(vex::forward);
  // Intake.stop();
  // this_thread::sleep_for(1500);
  // Intake.stop();
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

// this function finds a similar speed for the motorgroups
void auto_drive(int speed, double c) {
  float diff =
      std::abs(parent().rotation(rev)) - std::abs(child().rotation(rev));
  int sign = diff < 0 ? -1 : 1;
  int dV = sign * (diff)*speed * c; // c is a constant that needs tuning

  parent().spin(forward, speed, rpm);
  child().spin(forward, speed + dV, rpm);
}

void auto_drive_dist(int speed, int dist) {
  // reseting the rotation vals to 0
  parent().resetRotation();
  child().resetRotation();

  int revolutions =
      dist / (wheel_diameter() * M_PI); // converts inches into revolutions
  double gear_ratio = 1;
  // loops to slow down as the bot approaches its end position
  while (std::abs(parent().rotation(rev) * gear_ratio) < revolutions * 0.6) {
    auto_drive(speed, 0.1);
  }
  while (std::abs(parent().rotation(rev) * gear_ratio) < revolutions) {
    auto_drive(speed / 3, 0.1);
  }

  // this counteracts the momentum and stops the bot
  auto_drive(-speed / 2, 1);
  this_thread::sleep_for(50);
  auto_drive(0, 1);
}

void auto_turn(int speed) {
  parent().spin(forward, speed, rpm);
  child().spin(forward, -speed, rpm);
}

void auto_turn_deg(int speed, int deg) {
  float r = 0.2f; // this needs to be tuned --> it is basically
                  // motor_rev/bot_degree_turned

  parent().resetRotation();
  child().resetRotation();

  // slows down the bot as it approaches it makes rev
  // also gear_ratio doesn't matter because r accounts for it I think
  while (std::abs(parent().rotation(rev)) / r < deg * 0.8) {
    auto_turn(speed);
  }
  while (std::abs(parent().rotation(rev)) / r < deg * 0.8) {
    auto_turn(speed / 3);
  }

  auto_turn(-speed / 2);
  this_thread::sleep_for(50);
  auto_turn(0);
}

/*
To use this function, enter the speed, in rpms, and a r_test.
Decide on a degree amount you want the bot to turn: maybe 90 degrees.
Keep changing r_test until the bot actually reaches that 90 degrees.
*/
void tune_k_for_turn(int speed, int r_test) {
  parent().resetRotation();
  child().resetRotation();

  while (std::abs(parent().rotation(rev)) < r_test) {
    auto_turn(speed);
  }

  auto_turn(-speed / 2);
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
  case DISABLED:
    return;
    ;
  }
}