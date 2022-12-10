#pragma once
#include <vector>
#include "Math_Vector.h"
#include "xDrive.h"
using namespace std;

/*
Degrees to Radians
@return the radian equivilant of the input
*/
inline double DegreesToRadians(double degrees) {
    return degrees * (M_PI / 180);
}

// TODO: CHECK WHEEL RADIUS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
Radians to wheel rotations
@return the rotations needed for a wheel to rotate the inputted radians
*/
inline double RadiansToRotations(double radians) {
  return radians * M_PI * 1.125;
}


//TODO: CHECK!!!!!!!!!!!!!!!!!!!!!!!!
/*
Gets the robot radius
@return the radius of the robot in inches
*/
inline double robotRadius() {
  return 10.0; // dummy value
}

int driveTask(void* args);

int turnTask(void* args);

int intakeTask(void* args);

int rollerTask(void* args);
int flywheelTask(void* args);

int indexerTask(void* args);

// DO NOT INSTANTIATE
class Command {
public:
  bool is_complete_;
  bool is_blocking_;
  thread main;
  Command(bool a, bool b) : is_complete_(a), is_blocking_(b) {}
  inline bool isComplete() {
    return is_complete_;
  }
  inline void setComplete(bool a) {
    is_complete_ = a;
  };
};

void waitForComplete(Command* c);

class DriveCommand : public Command {
public:
  Command* wait_;
  Comp_Vector drive_vec_;
  double length_;
  /*
  Drives the robot
  @param drive the vector to drive: power as magnitude and direction, just like calling the drive() function
  @param length how far to drive in inches
  @param wait the command to wait for before executing this command
  @param async wether to execute the command on this thread
  */
  DriveCommand(Comp_Vector drive, double length, Command* wait, bool async) : Command(false, async), drive_vec_(drive), length_(length), wait_(wait)
  {
    if(async) {
      main = thread(&driveTask, this);
    } else {
      driveTask(this);
      is_complete_ = true;
    }
  }
};
class TurnCommand : public Command {
public:
  Command* wait_;
  double drive_;
  double radians_;
  /*
  Turns the robot
  @param drive_s how fast to turn
  @param length the radians to turn
  @param wait the command to wait for before executing this command
  @param async wether to execute the command on this thread
  */
  TurnCommand(double drive_s, double radians, Command* wait, bool async) : Command(false, async), drive_(drive_s), radians_(radians), wait_(wait)
  {
    if(async) {
      main = thread(&turnTask, this);
    } else {
      turnTask(this);
      is_complete_ = true;
    }
  }
};

class IntakeCommand : public Command {
public:
  Command* wait_;
  double drive_;
  bool on_;
  /*
  Controls the intake
  @param drive_s how fast to spin the intake
  @param on wether to turn the intake on
  @param wait the command to wait for before executing this command
  @param async wether to execute the command on this thread
  */
  IntakeCommand(double drive_s, bool on, Command* wait, bool async) : Command(false, async), drive_(drive_s), on_(on), wait_(wait)
  {
    if(!on) {
      Intake.stop();
    } else if(async) {
      main = thread(&intakeTask, this);
    } else {
      intakeTask(this);
      is_complete_ = true;
    }
  }
};
class RollerCommand : public Command {
public:
  Command* wait_;
  double distance_;
  /*
  Spins the roller
  @param d how much to turn the roller in degrees
  @param wait the command to wait for before executing this command
  @param async wether to execute the command on this thread
  */
  RollerCommand(double d, Command* wait, bool async) : Command(false, async), distance_(d), wait_(wait)
  {
    if(async) {
      main = thread(&rollerTask, this);
    } else {
      rollerTask(this);
      is_complete_ = true;
    }
  }
};
class FlywheelCommand : public Command {
public:
  Command* wait_;
  bool on_;
  /*
  Controls the flyweel
  @param on wether to turn the flywheel on or off
  @param wait the command to wait for before executing this command
  @param async wether to execute the command on this thread
  NOTE: is_complete_ is only true once the flywheel has spun up to over 550 rmp
  */
  FlywheelCommand(bool on, Command* wait, bool async) : Command(false, async), on_(on), wait_(wait)
  {
    if(!on) {
      Flywheel.stop();
    } else if(async) {
      main = thread(&flywheelTask, this);
    } else {
      flywheelTask(this);
      is_complete_ = true;
    }
  }
};
class IndexerCommand : public Command {
public:
  Command* wait_;
  /*
  Shoots one disk
  @param wait the command to wait for before executing this command
  @param async wether to execute the command on this thread
  NOTE: Also resets the indexer to its normal position
  */
  IndexerCommand(Command* wait, bool async) : Command(false, async), wait_(wait)
  {
    if(async) {
      main = thread(&indexerTask, this);
    } else {
      indexerTask(this);
      is_complete_ = true;
    }
  }
};