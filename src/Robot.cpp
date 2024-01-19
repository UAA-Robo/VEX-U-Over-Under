#include "Robot.h"

Robot::Robot()
{

    hw = new Hardware();
    rc = new RobotConfig(hw);
    tm = new Telemetry(hw, rc);
    
    userDrive = new UserDrive(hw, rc, tm); 
    autoDrive = new AutoDrive(hw, rc, tm);
}

void Robot::drive() {
    userDrive->drive();
}

void Robot::driveAuto() {
    autoDrive->drive();
}

void Robot::setup() {
   //autoDrive->setup_bot();
}
