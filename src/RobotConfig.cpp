#include "RobotConfig.h"

RobotConfig::RobotConfig(Hardware* hardware, bool hasSensor)
{
    ODOMETRY_CIRCUMFERENCE  = 2.75 * M_PI; // Odometry wheel circumference
    //WHEELCIRC = 3.25 * M_PI; // Drive train wheel circumference in inches --> CHECK: might be 3 inches!

    auto_drive_velocity_percent = 20;
    auto_rotate_velocity_percent = 20;
    
    if (hasSensor)
    {
        // set dimensions  
        ODOMETRY_LEFT_RIGHT_RADIUS =  7.0 / 2; //6.75/2; // Distance between left and right odometry wheels over 2
        ODOMETRY_BACK_RADIUS = 4.25; //3.5;
        DRIVETRAIN_WIDTH = 11.75; //12; // distance (in inch) between left and right side of the drivetrain (measured from the center of the wheels)
    }
    else
    {
         // Distance between left and right odometry wheels over 2. It was measured and then tuned 
         // to get accurate turning results
        ODOMETRY_LEFT_RIGHT_RADIUS =  6.45 / 2;
        ODOMETRY_BACK_RADIUS = 4.25; //3.5;
        DRIVETRAIN_WIDTH = 11.75; 
    }

    hw = hardware;
}