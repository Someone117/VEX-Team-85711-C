// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// BottomLeft           motor         4
// BottomRight          motor         9
// BackLeft             motor         3
// BackRight            motor         1
// Intake               motor         19
// Cata                 motor         20
// TopLeft              motor         5
// TopRight             motor         7
// CataSW               limit         A
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Vex Team 8511C                                            */
/*    Created:      Tue Aug 30 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "autonomous.h"
#include "tankDrive.h"
#include "vex.h"

using namespace vex;
using namespace std;

competition Competition;

controller::axis driveAxis() { return Controller1.Axis3; }
controller::axis turnAxis() { return Controller1.Axis1; }

void run(bool flipDrive) { // driving
  // Driving
  double deadZone = 15; // prevents drifting
  double driveAmt = driveAxis().position();
  // Turning
  double turnAmt = turnAxis().position();
  if (std::abs(turnAmt) < deadZone)
    turnAmt = 0;
  if (std::abs(driveAmt) < deadZone)
    driveAmt = 0;
  driveAndTurn(driveAmt, turnAmt, flipDrive);

  this_thread::sleep_for(50);
}

void init() { // init all torque, velocity and breaking
  BottomLeft.setMaxTorque(100, percent);
  BottomLeft.setStopping(coast);

  BottomRight.setMaxTorque(100, percent);
  BottomRight.setStopping(coast);

  BackLeft.setMaxTorque(100, percent);
  BackLeft.setStopping(coast);

  BackRight.setMaxTorque(100, percent);
  BackRight.setStopping(coast);

  TopLeft.setMaxTorque(100, percent);
  TopLeft.setStopping(coast);

  TopRight.setMaxTorque(100, percent);
  TopRight.setStopping(coast);

  Intake.setStopping(coast);

  Cata.setMaxTorque(100, percent);
  Cata.setStopping(hold);

  stopDrive(); // safety
}

void teleop() {
  init();
  bool flip = false;
  // bool endGameReturn = false;
  bool enableIntake = true;
  bool intakeOverride = false;
  double cataVoltage = 10;
  while (true) {
    // Drive/turn
    run(flip);

    // Cata
    if (CataSW.pressing()) {
      Cata.stop(); // cata is in position to shoot
      enableIntake = true;
      if (Controller1.ButtonL1.pressing()) { // shoot
        enableIntake = false;
        Cata.spin(vex::reverse, cataVoltage, volt);
        this_thread::sleep_for(200); // wait to let limit switch depress
      }
    } else {
      enableIntake = false;
      Cata.spin(vex::reverse, cataVoltage, volt); // bring the cata down
    }

    if (Controller1.ButtonRight.pressing() || !Cata.installed()) {
      intakeOverride = true;
    }

    // Intake and Rollers
    if (Controller1.ButtonR2.pressing() &&
        (enableIntake || intakeOverride)) { // spin intake in
      Intake.setVelocity(100, percent);
      Intake.spin(vex::forward);
    } else if (Controller1.ButtonL2.pressing() &&
               (enableIntake || intakeOverride)) { // spin intake out
      Intake.setVelocity(100, percent);
      Intake.spin(vex::reverse);
    } else {
      Intake.stop();
    }

    // Drive inversion
    if (Controller1.ButtonB.pressing()) {
      flip = !flip;
      this_thread::sleep_for(100); // pause to not flip the drive constantly
    }
    if (flip) {
      Controller1.rumble("-"); // Gives feedback to the driver
    }

    // debug
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("BackL: %.1f, Position: %.1f", BackLeft.temperature(),
                       BackLeft.voltage());
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("BottomL: %.1f, Voltage: %.1f", BottomLeft.temperature(),
                       BottomLeft.voltage());
    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print("TopL: %.1f, Voltage: %.1f", TopLeft.temperature(),
                       TopLeft.voltage());
    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print("BackR: %.1f, Voltage: %.1f", BackRight.temperature(),
                       BackRight.voltage());
    Brain.Screen.setCursor(5, 1);
    Brain.Screen.print("BottomR: %.1f, Voltage: %.1f",
                       BottomRight.temperature(), BottomRight.voltage());
    Brain.Screen.setCursor(6, 1);
    Brain.Screen.print("TopR: %.1f, Voltage: %.1f", TopRight.temperature(),
                       TopRight.voltage());
    Brain.Screen.setCursor(7, 1);
    Brain.Screen.print("Intake: %.1f, Voltage: %.1f", Intake.temperature(),
                       Intake.voltage());
    Brain.Screen.setCursor(8, 1);
    Brain.Screen.print("Cata: %.1f, Voltage: %.1f", Cata.temperature(),
                       Cata.voltage());
    Brain.Screen.setCursor(9, 1);
    Brain.Screen.print("SW: %s", CataSW.pressing() ? "true" : "false");

    // Endgame
    // if (Controller1.ButtonY.pressing()) {
    //   if (endGameReturn) {
    //     EndGame.startSpinFor(vex::reverse, 130, degrees);
    //     endGameReturn = false;
    //   }
    // }
  }
  thread::interruptAll();
}
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  init();

  // Skills test
  // Controller1.ButtonA.pressed([] () { auton(SKILLS_2); });

  // Competition.autonomous([]() { auton(R); }); // Which auton to do
  // Competition.drivercontrol(teleop);
  auto_drive_dist(12); 
}