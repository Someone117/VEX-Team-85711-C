#pragma once
#include "vex.h"

enum autons { L, R, SKILLS, DISABLED, L_SIMPLE };
void auton_skills();
void auton_left();
void auton_right();
void auton(autons aut);
void auton_left_simple();