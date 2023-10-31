#include "RobotConfig.h"

RobotConfig::RobotConfig(Hardware* hardware, bool hasSensor)
{
    if (hasSensor)
    {
        // set dimensions  
        ODOMETRY_LEFT_RIGHT_RADIUS = 6.75/2;
        ODOMETRY_BACK_RADIUS = 4.25;
        DRIVETRAINWIDTH = 12; // distance (in inch) between left and right side of the drivetrain (measured from the center of the wheels)
    }
    else
    {
        // set dimensions
        ODOMETRY_LEFT_RIGHT_RADIUS = 6.75/2;
        ODOMETRY_BACK_RADIUS = 4.25;
        DRIVETRAINWIDTH = 12;
    }

    hw = hardware;
}