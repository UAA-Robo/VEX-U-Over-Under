#include "RobotConfig.h"

RobotConfig::RobotConfig(Hardware* hardware)
{
    hw = hardware;
    ODOMETRY_CIRCUMFERENCE  = 2.75 * M_PI; // Odometry wheel circumference
    //WHEELCIRC = 3.25 * M_PI; // Drive train wheel circumference in inches --> CHECK: might be 3 inches!

    auto_drive_velocity_percent = 20;
    auto_rotate_velocity_percent = 20;
    
    if (hw->distanceSensor.installed()) // ONLY Scratette has distance sensor
    {
        ROBOT = SCRATETTE;

        // Odometry wheel are flipped from Scrat
        hw->left_odometry.setReversed(false);
        hw->left_odometry.setReversed(false);
        hw->back_odometry.setReversed(true);

        /*
        ----SET DIMENSIONS (in) --------------------------------------------------------------
        */
        // Distance between left and right odometry wheels over 2. It was measured and then tuned 
        // to get accurate turning results. (measured from the center of the wheels)
        ODOMETRY_LEFT_RIGHT_RADIUS =  4.0 / 2; 

        // Distance between horizontal odometry wheel and horizontal center line
        ODOMETRY_BACK_RADIUS = 3.25; 

        ODOMETRY_BACK_OFFSET = 3.4;

        // Distance (in inch) between left and right side of the drivetrain 
        // (measured from the center of the wheels)
        DRIVETRAIN_WIDTH = 13.25; 

        // Max width
        ACTUAL_WIDTH = 17.6;  
    }
    else
    {
        std::cout << "Robot is SCRAT\n";
        ROBOT = SCRAT;

        /*
        ----SET DIMENSIONS (in) --------------------------------------------------------------
        */
         // Distance between left and right odometry wheels over 2. It was measured and then tuned 
         // to get accurate turning results
        ODOMETRY_LEFT_RIGHT_RADIUS =  6.45 / 2;

        // Distance between horizontal odometry wheel and horizontal center line
        ODOMETRY_BACK_RADIUS = 4.25; 
        ODOMETRY_BACK_OFFSET = 0;

        // Distance (in inch) between left and right side of the drivetrain 
        // (measured from the center of the wheels)
        DRIVETRAIN_WIDTH = 11.75;

        // Max width
        ACTUAL_WIDTH = 14.50; 

    }
    DRIVETRAIN_RADIUS = DRIVETRAIN_WIDTH * sqrt(2) / 2.0;
    ACTUAL_RADIUS = ACTUAL_WIDTH * sqrt(2) / 2.0;

    hw = hardware;
}