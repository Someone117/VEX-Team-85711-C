#pragma once
#include "Math_Vector.h"

void stopDrive();
void turn(double v);
void drive(double v, bool flipDrive);
void driveAndTurn(double d, double t, bool flipDrive);
double logDrive(double v, double pow);

void driveFor(double voltage, double distance);
void turnFor(double voltage, double distance);