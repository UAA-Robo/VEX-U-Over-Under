#include "Robot.h"

Robot::Robot()
{

    hw = new Hardware();


    // check if robot has sensor
    bool hasSensor;
    
    hasSensor = hw->distanceSensor.installed(); 
    rc = new RobotConfig(hw, hasSensor);
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
