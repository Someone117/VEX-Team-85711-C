#pragma once
#include "Math_Vector.h"

void stopDrive();
void turn(double v, bool flipDrive);
void drive(double v, bool flipDrive);
double logDrive(double v, int pow);
inline int getLogPower() { return 3; }
// testing
void driveEncode(double v, double distance);
void turnEncode(double v, double distance);