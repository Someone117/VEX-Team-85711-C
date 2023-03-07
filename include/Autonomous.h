#pragma once
#include "vex.h"
/*  For Angles (positive is to the right):
    +90: max = 7.1; min = 4
    -90: max = -3; min = -4.998
    +45: max = 6.17; min = 4
    -45: max = -2.5; min = -4
    Constants: kP = 25.955; kI = -0.5; kD = 10


    For Distance:
    +24: max = 2.5; min = 7?
    Constants kP = -2; kI = 0; kD = 70

*/
enum config { 
  AUTO_SKILLS, AUTO_SKILLS_SAFE, MATCH_L, MATCH_R, DRIVER_SKILLS, TESTER, NO_AUTO
};
void auton_skills();
void auton_left();
void auton_right();
void auton(config aut);
void auton_left_simple();
void auton_skills_2();
double volts_checked(double volts, double max, double min);
void auto_drive(double volts, double MAX_V, double c);
void auto_drive_dist(int dist, double MAX_V, double MIN_V, double kP, double kI, double kD);
void auto_turn(double volts);
void auto_turn_deg_PID(int deg, double kP, double kI, double kD, double MAX_V, double min);