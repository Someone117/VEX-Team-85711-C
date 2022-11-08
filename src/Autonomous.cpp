#include "Autonomous.h"
#include <algorithm>

//TODO: CHANGE THE TURN STOPPING TO BE RADIANS INSTEAD OF LENGTH (CHANGE THE COUNt STUFF TOO)


/* explanation:
the c is nullptr if we don't need to wait, waits for the command if we need to
*/
void waitForComplete(Command* c) {
  if(c == nullptr) return;
  while(!(*c).isComplete()) {
    vex::task::sleep(50);
  }
}

/* explanation:
execute all commands in the list
*/
void execute(vector<Command> cs) {
  for(vector<Command>::iterator i = cs.begin(); i != cs.end(); ++i) {
    (*i).execute();
  }
}


/*explanation:
Setup inital values of motors
Setup differences, values that will hold the change in degrees of each motor
Cast and dereference the pointer to the DriveCommand that we passed as args
Set the motors to break (allows us to stop where we are without too much coasting due to momentum)
Setup motor vectors (motor position with magnitude of 1)
Calculate the time it will take to go the distance (used to stop loop from going infinitly):
max rpm * percent of rpm we drive * length to drive * radius / 60 (m to s) * 10 (10 loops per sec)
While motor vectors added together produce a magnitude less than length to drive (should be driving drive_vec_ direction):
  Check for count overflow
  Drive at the correct angle and speed
  Wait 0.1 s, allows time to drive and time for other tasks to run
  Append our motor distance values
  Add 1 to count
Stop motors
Reset stopping mode
*/
int driveTask(void* args) {
  DriveCommand* dp = static_cast<DriveCommand*>(args);
  DriveCommand d = *dp;
  waitForComplete(d.wait_);
 
  const double FL = FrontLeft.position(degrees);
  const double BL = BackLeft.position(degrees);
  const double BR = BackRight.position(degrees);
  const double FR = FrontRight.position(degrees);
  
  double d1 = 0;
  double d2 = 0;
  double d3 = 0;
  double d4 = 0;

  FrontLeft.setStopping(brake);
  FrontRight.setStopping(brake);
  BackLeft.setStopping(brake);
  BackRight.setStopping(brake);
  Mag_Vector v1(1, M_PI/4.0);
  Mag_Vector v2(1, 7.0*M_PI/4.0);
  Mag_Vector v3(1, 3.0*M_PI/4.0);
  Mag_Vector v4(1, 5.0*M_PI/4.0);
  // might need to check
  double speed = 200.0 * d.drive_vec_.get_mag() / 100.0;
  double time = speed * 1.25 * d.length_ / 60.0;
  double times = time * 10.0;
  int count = 0;
  while(std::abs(RadiansToRotations(DegreesToRadians((v1+v2+v3+v4).get_mag()/4.0))) < d.length_) {
    if(count > times) break;
    drive(d.drive_vec_);
    vex::task::sleep(100);
    d1 = FrontLeft.position(degrees)-FL;
    d2 = BackLeft.position(degrees)-BL;
    d3 = BackRight.position(degrees)-BR;
    d4 = FrontRight.position(degrees)-FR;
    v1.set_mag(d1);
    v2.set_mag(d2);
    v3.set_mag(d3);
    v4.set_mag(d4);
    count++;
  }
  FrontLeft.stop();
  FrontRight.stop();
  BackLeft.stop();
  BackRight.stop();
  FrontLeft.setStopping(coast);
  FrontRight.setStopping(coast);
  BackLeft.setStopping(coast);
  BackRight.setStopping(coast);
  d.setComplete(true);
  return 0;
}

/*explanation:
Setup inital values of motors
Setup differences, values that will hold the change in degrees of each motor
Cast and dereference the pointer to the DriveCommand that we passed as args
Set the motors to break (allows us to stop where we are without too much coasting due to momentum)
Calculate the distance the wheels will need to move to turn the given radians:
radius of the robot * radians to turn
Calculate the time it will take to go the distance (used to stop loop from going infinitly):
max rpm * percent of rpm we drive * length to drive * radius / 60 (m to s) * 10 (10 loops per sec)
While we are driving less than the calculated distance:
  Check for count overflow
  Drive at the correct angle and speed
  Wait 0.1 s, allows time to drive and time for other tasks to run
  Append our motor distance values
  Add 1 to count
Stop motors
Reset stopping mode
*/
int turnTask(void* args) {
  TurnCommand* dp = static_cast<TurnCommand*>(args);
  TurnCommand d = *dp;
  waitForComplete(d.wait_);
  
  double FL = FrontLeft.position(degrees);
  double BL = BackLeft.position(degrees);
  double BR = BackRight.position(degrees);
  double FR = FrontRight.position(degrees);
  double d1 = 0;
  double d2 = 0;
  double d3 = 0;
  double d4 = 0;
  FrontLeft.setStopping(brake);
  FrontRight.setStopping(brake);
  BackLeft.setStopping(brake);
  BackRight.setStopping(brake);

  double distance = d.radians_ * robotRadius();
  double speed = 200.0 * std::abs(d.drive_);
  double time = speed * distance / 60.0;
  double times = time * 10.0 * 1.25;
  int count = 0;
  while(std::abs(RadiansToRotations(DegreesToRadians(abs((d1+d2+d3+d4)/4.0)))) < distance) {
    if(count > times) break;
    turn(d.drive_);
    d1 = FrontLeft.position(degrees)-FL;
    d2 = BL-BackLeft.position(degrees);
    d3 = BackRight.position(degrees)-BR;
    d4 = FR-FrontRight.position(degrees);
    count++;
    vex::task::sleep(100);
  }
  FrontLeft.stop();
  FrontRight.stop();
  BackLeft.stop();
  BackRight.stop();
  FrontLeft.setStopping(coast);
  FrontRight.setStopping(coast);
  BackLeft.setStopping(coast);
  BackRight.setStopping(coast);
  d.setComplete(true);
  return 0;
}

int intakeTask(void* args) {
  IntakeCommand* ip = static_cast<IntakeCommand*>(args);
  IntakeCommand i = *ip;
  waitForComplete(i.wait_);

  return 0;
}

int rollerTask(void* args) {
  RollerCommand* rp = static_cast<RollerCommand*>(args);
  RollerCommand r = *rp;
  waitForComplete(r.wait_);
  return 0;
}


/* explanation:
Cast and dereference the pointer to the FlywheelCommand that we passed as args
if the flywheel should be turned on {
  start spinning it
  if the flywheel should block the thread {
    initialize a counter variable to track elapsed seconds
    wait for the flyweel to spin up, or wait 3 seconds, whichever is shorter
  }
} else (AKA: flywheel should be turrned off) {
  turn off the flywheel
  if the flywheel should block the tread {
    initialize a counter variable to track elapsed seconds
    wait for the flyweel to spin down, or wait 4 seconds, whichever is shorter
  }
}
*/
int flywheelTask(void* args) {
  FlywheelCommand* fp = static_cast<FlywheelCommand*>(args);
  FlywheelCommand f = *fp;
  waitForComplete(f.wait_);

  if(f.on_) {
    Flywheel.spin(vex::forward);
    if(f.is_blocking_) {
      int count = 0;
      while(Flywheel.velocity(rpm) < 550) {
        if(count > 30) return 1;
        count++;
        vex::task::sleep(100);
      }
    }
  } else {
    Flywheel.stop();
    if(f.is_blocking_) {
      int count = 0;
      while(Flywheel.velocity(rpm) > 10) {
        if(count > 4) return 1;
        count++;
        vex::task::sleep(100);
      }
    }
  }
  f.setComplete(true);
  return 0;
}


int indexerTask(void* args) {
  IndexerCommand* ip = static_cast<IndexerCommand*>(args);
  IndexerCommand i = *ip;
  waitForComplete(i.wait_);
  
  return 0;
}