#include "tankDrive.h"
#include "vex.h"

void stopDrive() {
  FrontLeft.stop();
  FrontRight.stop();
  BackLeft.stop();
  BackRight.stop();
  TopLeft.stop();
  TopRight.stop();
}
double logDrive(double v, int pow) {
  return (std::pow(v, pow) / std::pow(127, pow)) * 100;
}
void drive(double v, bool flipDrive) {
  if (flipDrive) {
    v *= -1;
  }
  v = logDrive(v, getLogPower());
  FrontLeft.spin(v > 0 ? vex::forward : vex::reverse, v, percent);
  FrontRight.spin(v > 0 ? vex::forward : vex::reverse, v, percent);
  BackLeft.spin(v > 0 ? vex::forward : vex::reverse, v, percent);
  BackRight.spin(v > 0 ? vex::forward : vex::reverse, v, percent);
  TopLeft.spin(v > 0 ? vex::forward : vex::reverse, v, percent);
  TopRight.spin(v > 0 ? vex::forward : vex::reverse, v, percent);
}
void turn(double v, bool flipDrive) {
  if (flipDrive) {
    v *= -1;
  }
  v = logDrive(v, getLogPower());
  FrontLeft.spin(v > 0 ? vex::forward : vex::reverse, v, percent);
  FrontRight.spin(v < 0 ? vex::forward : vex::reverse, v, percent);
  BackLeft.spin(v > 0 ? vex::forward : vex::reverse, v, percent);
  BackRight.spin(v < 0 ? vex::forward : vex::reverse, v, percent);
  TopLeft.spin(v > 0 ? vex::forward : vex::reverse, v, percent);
  TopRight.spin(v < 0 ? vex::forward : vex::reverse, v, percent);
}
void turnEncode(double v, double distance) {
  double fl = FrontLeft.position(degrees);
  double fr = FrontRight.position(degrees);
  double bl = BackLeft.position(degrees);
  double br = BackRight.position(degrees);
  double tl = TopLeft.position(degrees);
  double tr = TopRight.position(degrees);
  double newDistance = (distance / M_2_PI) / 2; // check
  double avgL = (fl + bl + tl - TopLeft.position(degrees) -
                 BackLeft.position(degrees) - FrontLeft.position(degrees)) /
                3.0;
  double avgR = (fr + br + tr - TopRight.position(degrees) -
                 -BackRight.position(degrees) - FrontRight.position(degrees)) /
                3.0;
  while (std::abs(avgL) + std::abs(avgR) / 2.0 < std::abs(newDistance)) {
    turn(v, false);
    avgR = (fr + br + tr - TopRight.position(degrees) -
            -BackRight.position(degrees) - -FrontRight.position(degrees)) /
           3.0;
    avgL = (fl + bl + tl - TopLeft.position(degrees) -
            BackLeft.position(degrees) - FrontLeft.position(degrees)) /
           3.0;
  }
  stopDrive();
}
void driveEncode(double v, double distance) {
  double fl = FrontLeft.position(degrees);
  double fr = FrontRight.position(degrees);
  double bl = BackLeft.position(degrees);
  double br = BackRight.position(degrees);
  double tl = TopLeft.position(degrees);
  double tr = TopRight.position(degrees);
  double newDistance = (distance / M_2_PI) / 2; // check
  double avg = (fl + fr + bl + br + tl + tr - TopRight.position(degrees) -
                TopLeft.position(degrees) - BackRight.position(degrees) -
                BackLeft.position(degrees) - FrontRight.position(degrees) -
                FrontLeft.position(degrees)) /
               6.0;
  while (std::abs(avg) < std::abs(newDistance)) {
    drive(v, false);
    avg = (fl + fr + bl + br + tl + tr - TopRight.position(degrees) -
           TopLeft.position(degrees) - BackRight.position(degrees) -
           BackLeft.position(degrees) - FrontRight.position(degrees) -
           FrontLeft.position(degrees)) /
          6.0;
  }
  stopDrive();
}