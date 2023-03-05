#pragma once
#include "vex.h"

enum autons { L, R, SKILLS, DISABLED, L_SIMPLE, SKILLS_2 };
void auton_skills();
void auton_left();
void auton_right();
void auton(autons aut);
void auton_left_simple();
void auton_skills_2();

inline double wheel_diameter() { return 3.25; }
inline motor_group parent() { return LeftDrive; }
inline motor_group child() { return RightDrive; }
// int encoderTicks() {
//   return 300;
// }