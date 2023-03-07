#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor BottomLeft = motor(PORT8, ratio6_1, true);
motor BottomRight = motor(PORT13, ratio6_1, false);
motor BackLeft = motor(PORT15, ratio6_1, true);
motor BackRight = motor(PORT16, ratio6_1, false);
motor Intake = motor(PORT19, ratio18_1, false);
motor Cata = motor(PORT20, ratio36_1, false);
motor TopLeft = motor(PORT4, ratio6_1, false);
motor TopRight = motor(PORT7, ratio6_1, true);
limit CataSW = limit(Brain.ThreeWirePort.A);
digital_out LeftEndGame = digital_out(Brain.ThreeWirePort.B);
digital_out RightEndGame = digital_out(Brain.ThreeWirePort.C);
motor_group LeftDrive = motor_group(BottomLeft, TopLeft, BackLeft);
motor_group RightDrive = motor_group(BottomRight, TopRight, BackRight);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // nothing to initialize
}