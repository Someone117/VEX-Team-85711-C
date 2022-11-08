#pragma once
#include <vector>
#include "Math_Vector.h"
#include "xDrive.h"
using namespace std;

inline double DegreesToRadians(double degrees) {
    return degrees * (M_PI / 180);
}

// TODO: CHECK WHEEL RADIUS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
inline double RadiansToRotations(double radians) {
  return radians * M_PI * 1.125;
}


//TODO: CHECK!!!!!!!!!!!!!!!!!!!!!!!!
//inches
inline double robotRadius() {
  return 10.0; // dummy value
}

int driveTask(void* args);

int turnTask(void* args);

int intakeTask(void* args);

int rollerTask(void* args);
int flywheelTask(void* args);

int indexerTask(void* args);

class Command {
public:
  int type;
  bool is_complete_;
  bool is_blocking_;
  inline bool isComplete() {
    return is_complete_;
  }
  inline void setComplete(bool a) {
    is_complete_ = a;
    type = 0;
  };
};

void waitForComplete(Command* c);
void execute(vector<Command> cs);

class DriveCommand : public Command {
public:
  Command* wait_;
  Comp_Vector drive_vec_;
  double length_;
  DriveCommand(Comp_Vector drive, double length, Command* wait, bool async) : drive_vec_(drive), length_(length), wait_(wait)
  {
    is_complete_ = false;
    is_blocking_ = async;
    type = 1;
  }
  inline void execute() {
    if(is_blocking_) {
      vex::task t(&driveTask, this);
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
  TurnCommand(double drive_s, double radians, Command* wait, bool async) : drive_(drive_s), radians_(radians), wait_(wait)
  {
    is_complete_ = false;
    is_blocking_ = async;
    type = 2;
  }
  inline void execute() {
    if(is_blocking_) {
      vex::task t(&turnTask, this);
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
  double length_;
  IntakeCommand(double drive_s, double length, Command* wait, bool async) : drive_(drive_s), length_(length), wait_(wait)
  {
    is_complete_ = false;
    is_blocking_ = async;
    type = 3;
  }
  inline void execute() {
    if(is_blocking_) {
      vex::task t(&intakeTask, this);
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
  RollerCommand(double d, Command* wait, bool async) : distance_(d), wait_(wait)
  {
    is_complete_ = false;
    is_blocking_ = async;
    type = 4;
  }
  inline void execute() {
    if(is_blocking_) {
      vex::task t(&rollerTask, this);
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
  FlywheelCommand(bool on, Command* wait, bool async) : on_(on), wait_(wait)
  {
    is_complete_ = false;
    is_blocking_ = async;
    type = 5;
  }
  inline void execute() {
    if(is_blocking_) {
      vex::task t(&flywheelTask, this);
    } else {
      flywheelTask(this);
      is_complete_ = true;
    }
  }
};
class IndexerCommand : public Command {
public:
  Command* wait_;
  IndexerCommand(Command* wait, bool async) : wait_(wait)
  {
    is_complete_ = false;
    is_blocking_ = async;
    type = 6;
  }
  inline void execute() {
    if(is_blocking_) {
      vex::task t(&indexerTask, this);
    } else {
      indexerTask(this);
      is_complete_ = true;
    }
  }
};