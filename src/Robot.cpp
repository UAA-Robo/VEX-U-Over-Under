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

    // create map
    mp = new Map(tm, autoDrive);

    // set starting zone
    //for (int i = 22; i < 26; ++i) { if (mp->mapElements[i]->inZone(tm->odometry_position)) {zone = mp->mapElements[i]; break;} }
}


void Robot::drive() {
    userDrive->drive();
}


void Robot::driveAuto() {
    autoDrive->drive();
}
