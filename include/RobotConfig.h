#pragma once
#include "Hardware.h"

/// @brief    Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw);
    const float ODOMETRY_LEFT_RIGHT_RADIUS = 2; // TODO: change
    const float ODOMETRY_BACK_RADIUS = 10;  // TODO: change
    const float ODOMETRY_CIRCUMFERENCE  = 2.75 * M_PI;
    const double WHEELCIRC = 4 * M_PI; // Drive train wheel circumference in inches
    double auto_drive_velocity_percent = 45;
    double auto_rotate_velocity_percent = 20;
    const double DRIVETRAINWIDTH = 14.5; // distance (in inch) between left and right side of the drivetrain (measured from the center of the wheels)

private:
    Hardware* hw;

};
