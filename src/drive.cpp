#include "vex.h"
#include "drive.h"

using namespace std;

using namespace vex;

int velocityAt(int position)
{
  int absPos = abs(position);

  return absPos < minimumDriveVelocity ? minimumDriveVelocity : absPos;
}

void powerLeft(int position)
{
  if(position >= -1 * deadzone && position <= deadzone) LeftMotors.stop();
  else
  {
    vex::directionType direction = position < 0 ? vex::reverse : vex::forward;

    LeftMotors.setVelocity(velocityAt(position), percent);
    LeftMotors.spin(direction);
  }
}

void powerRight(int position)
{
  if(position >= -1 * deadzone && position <= deadzone) RightMotors.stop();
  else
  {
    vex::directionType direction = position < 0 ? vex::reverse : vex::forward;

    RightMotors.setVelocity(abs(position), percent);
    RightMotors.spin(direction);
  }
}