using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor FrontLeft;
extern motor FrontRight;
extern motor BackLeft;
extern motor BackRight;
extern motor Indexer;
extern motor Intake;
extern motor Flywheel;
extern motor EndGame;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );