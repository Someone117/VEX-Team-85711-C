#pragma once
#include "vex.h"

enum autons { L, R, SKILLS, DISABLED };
void index();
void auton_skills();
void auton_left();
void auton_right();
void auton(autons aut);