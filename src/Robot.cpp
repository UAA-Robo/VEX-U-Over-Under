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
    for (int i = 22; i < 26; i++)
    {
        Obstacle* newZone =  static_cast<Obstacle*>(mp->mapElements[i]);
        if (newZone->inZone(tm->odometry_position))
        {
            zone = newZone;
            break;
        }
    }
}


void Robot::drive() {
    userDrive->drive();
}


void Robot::driveAuto() {
    autoDrive->drive();
}
