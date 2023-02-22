using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor BottomLeft;
extern motor BottomRight;
extern motor BackLeft;
extern motor BackRight;
extern motor Intake;
extern motor Cata;
extern motor TopLeft;
extern motor TopRight;
extern limit CataSW;
extern motor_group LeftDrive;
extern motor_group RightDrive;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);