#pragma once
#include "vex.h"

enum config { 
  AUTO_SKILLS, AUTO_SKILLS_SAFE, MATCH_L, MATCH_R, DRIVER_SKILLS, TESTER, NO_AUTO
};
void auton_skills();
void auton_left();
void auton_right();
void auton(config c);
void auton_left_simple();
void auton_skills_safe();

void tune_k_for_turn();
void auto_turn_deg();
void auto_turn();
void auto_drive_dist();
void auto_drive();

inline double wheel_diameter() { return 3.25; }
inline motor_group parent() { return LeftDrive; }
inline motor_group child() { return RightDrive; }
// int encoderTicks() {
//   return 300;
// }