#pragma once
#include "vex.h"

enum autons { L, R, SKILLS, DISABLED, L_SIMPLE, SKILLS_2 };
void auton_skills();
void auton_left();
void auton_right();
void auton(autons aut);
void auton_left_simple();
void auton_skills_2();
double volts_checked(double volts, double max);
void auto_drive(double volts, double MAX_V, double c);
void auto_drive_dist(int dist);
void auto_turn(double volts);
void auto_turn_deg(double volts, int deg);
void tune_r_for_turn(double volts, int r_test);