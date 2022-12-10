#pragma once
#include "vex.h"
#include "Math_Vector.h"

double map(double x, double in_min, double in_max, double out_min, double out_max);
void turn(double v);
void drive(Comp_Vector driveVec, bool flipDrive);
void stopDrive();