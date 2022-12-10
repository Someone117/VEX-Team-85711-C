#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor FrontLeft = motor(PORT5, ratio18_1, false);
motor FrontRight = motor(PORT19, ratio18_1, true);
motor BackLeft = motor(PORT2, ratio18_1, true);
motor BackRight = motor(PORT13, ratio18_1, false);
motor Indexer = motor(PORT1, ratio18_1, false);
motor Intake = motor(PORT6, ratio36_1, false);
motor Flywheel = motor(PORT16, ratio6_1, true);
motor EndGame = motor(PORT8, ratio18_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}