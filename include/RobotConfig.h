#pragma once
#include "Hardware.h"

/// @brief    Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw);
<<<<<<< HEAD
    const float ODOMETRY_LEFT_RIGHT_RADIUS = 6.75/2; // TODO: change
    const float ODOMETRY_BACK_RADIUS = 4.25;  // TODO: change
    const float ODOMETRY_CIRCUMFERENCE  = 3 * M_PI;
=======
    const float ODOMETRY_LEFT_RIGHT_RADIUS = 2; // TODO: change
    const float ODOMETRY_BACK_RADIUS = 10;  // TODO: change
    const float ODOMETRY_CIRCUMFERENCE  = 2.75 * M_PI;
    const double WHEELCIRC = 3.25 * M_PI; // Drive train wheel circumference in inches
    double auto_drive_velocity_percent = 20;
    double auto_rotate_velocity_percent = 20;
    const double DRIVETRAINWIDTH = 12; // distance (in inch) between left and right side of the drivetrain (measured from the center of the wheels)

>>>>>>> 8616b17f4cc40b1b05cff1138dd07e69a997eec0
private:
    Hardware* hw;
};
