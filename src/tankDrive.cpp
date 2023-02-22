#include "tankDrive.h"
#include "vex.h"

void stopDrive() {
  LeftDrive.stop(brake);
  RightDrive.stop(brake);
}
double logDrive(double v, double pow) {
  if (v > 0) {
    return (std::pow(std::abs(v), pow) / std::pow(100, pow)) * 12.8;
  } else {
    return -1 * (std::pow(std::abs(v), pow) / std::pow(100, pow)) * 12.8;
  }
}
void drive(double v, bool flipDrive) {
  if (flipDrive) {
    v *= -1;
  }

  v = logDrive(v, 2);
  LeftDrive.spin(vex::forward, v, volt);
  RightDrive.spin(vex::forward, v, volt);
}
void driveAndTurn(double d, double t, bool flipDrive) {
  if (flipDrive) {
    d *= -1;
  }
  d = logDrive(d, 2);
  t = logDrive(-t, 3.5);
  double r = d + t;
  double l = d - t;

  if (r > 12.8)
    r = 12.8;
  else if (r < -12.8)
    r = -12.8;

  if (l > 12.8)
    l = 12.8;
  else if (l < -12.8)
    l = -12.8;
  LeftDrive.spin(vex::forward, l, volt);
  RightDrive.spin(vex::forward, r, volt);
}
void turn(double v) {
  v = logDrive(v, 3.5);
  LeftDrive.spin(vex::forward, -v, volt);
  RightDrive.spin(vex::forward, v, volt);
}

void driveFor(double voltage, double distance)  {
  // clamp voltage
  if (voltage > 12.8)
    voltage = 12.8;
  else if (voltage < -12.8)
    voltage = -12.8;
  if(distance == 0) return;
  LeftDrive.resetPosition();
  RightDrive.resetPosition();
  double distance2 = 0;
  while(distance > distance2) {
    LeftDrive.spin(vex::forward, voltage, volt);
    RightDrive.spin(vex::forward, voltage, volt);
    distance2 = std::abs((LeftDrive.position(rotationUnits::rev) + RightDrive.position(rotationUnits::rev))/2.0);
  }
  stopDrive();
}
void turnFor(double voltage, double distance) {
  // clamp voltage
  if (voltage > 12.8)
    voltage = 12.8;
  else if (voltage < -12.8)
    voltage = -12.8;
  if(distance == 0) return;
  LeftDrive.resetPosition();
  RightDrive.resetPosition();
  double distance2 = 0;
  while(distance > distance2) {
    LeftDrive.spin(vex::forward, -voltage, volt);
    RightDrive.spin(vex::forward, voltage, volt);
    distance2 = std::abs((-LeftDrive.position(rotationUnits::rev) + RightDrive.position(rotationUnits::rev))/2.0);
  }
  stopDrive();
}