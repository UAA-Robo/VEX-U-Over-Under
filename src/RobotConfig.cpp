#include "RobotConfig.h"

RobotConfig::RobotConfig(Hardware* hardware)
{
    hw = hardware;
    ODOMETRY_CIRCUMFERENCE  = 2.75 * M_PI; // Odometry wheel circumference
    //WHEELCIRC = 3.25 * M_PI; // Drive train wheel circumference in inches --> CHECK: might be 3 inches!

    auto_drive_velocity_percent = 20;
    auto_rotate_velocity_percent = 20;
    
    if (hw->distanceSensor.installed()) // ONLY Scrattete has distance sensor
    {
        ROBOT = SCRATETTE;
        // set dimensions  
        ODOMETRY_LEFT_RIGHT_RADIUS =  7.0 / 2; //6.75/2; // Distance between left and right odometry wheels over 2
        ODOMETRY_BACK_RADIUS = 4.25; //3.5;
        DRIVETRAIN_WIDTH = 11.75; //12; // distance (in inch) between left and right side of the drivetrain (measured from the center of the wheels)
        ACTUAL_WIDTH = 999999999999999999999.9; //TODO: <<<
    }
    else
    {
        std::cout << "Robot is SCRAT\n";
        ROBOT = SCRAT;
         // Distance between left and right odometry wheels over 2. It was measured and then tuned 
         // to get accurate turning results
        ODOMETRY_LEFT_RIGHT_RADIUS =  6.45 / 2;
        ODOMETRY_BACK_RADIUS = 4.25; //3.5;
        DRIVETRAIN_WIDTH = 11.75;
        ACTUAL_WIDTH = 14.50; // inches

    }
    DRIVETRAIN_RADIUS = DRIVETRAIN_WIDTH * sqrt(2) / 2.0;
    ACTUAL_RADIUS = ACTUAL_WIDTH * sqrt(2) / 2.0;

    hw = hardware;
}