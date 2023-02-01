#include "autonomous.h"
#include "vex.h"
#include "xDrive.h"

void auton_skills() {}

void auton_left() {
  // Flywheel.spin(vex::forward);

  // // Drives into and rolls the close roller
  // Comp_Vector a(-50, 0);
  // drive(a, false);
  // this_thread::sleep_for(500);
  // stopDrive();
  // Intake.spin(vex::reverse, 70, percent);
  // this_thread::sleep_for(250);
  // Intake.stop();

  // // gets off the roller to position and shoots
  // a.set_mag(a.get_mag() * -1);
  // drive(a, false);
  // this_thread::sleep_for(500);
  // // shoot three times, but we will have 2 disks
  // index();
  // index();
  // index();
  // Flywheel.stop();

  // // should uno reverse and drive into the other roller backwards
  // turn(-50, true);
  // this_thread::sleep_for(500);
  // drive(a, false);
  // this_thread::sleep_for(750);
  // stopDrive();
  // Intake.spin(vex::reverse, 70, percent);
  // this_thread::sleep_for(250);
  // Intake.stop();
}

void auton_right() {
  // Flywheel.spin(vex::forward);

  // // drive left
  // Comp_Vector a(-50, 0);
  // Comp_Vector b(0, -50);
  // // drive left to get to roller
  // drive(b, false);
  // this_thread::sleep_for(500);
  // stopDrive();
  // // drive backwards and spin roller
  // drive(a, false);
  // this_thread::sleep_for(500);
  // stopDrive();
  // Intake.spin(vex::reverse, 70, percent);
  // this_thread::sleep_for(250);
  // Intake.stop();
  // // shoot three times, but we will have 2 disks
  // index();
  // index();
  // index();
  // Flywheel.stop();
}

void auton_left_simple() {
  // Flywheel.setVelocity(100, percent);
  // Flywheel.spin(vex::forward);
  // // back into roller
  // Comp_Vector a(-50, 0);
  // drive(a, false);
  // this_thread::sleep_for(500);
  // stopDrive();

  // // roller
  // Intake.spin(vex::reverse, 100, percent);
  // this_thread::sleep_for(500);
  // Intake.stop();
  // stopDrive();
  // this_thread::sleep_for(5000);
  // stopDrive();
  // index();
  // index();
  // index();
  // index();
  // index();
  // Indexer.stop();
  // Flywheel.stop();
  // stopDrive();
}

void auton(autons aut) { // choose which auton
  switch (aut) {
  case L:
    auton_left();
  case R:
    auton_right();
  case SKILLS:
    auton_skills();
  case L_SIMPLE:
    auton_left_simple();
  case DISABLED:;
  }
}