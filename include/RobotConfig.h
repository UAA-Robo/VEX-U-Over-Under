#pragma once
#include "Hardware.h"

/// @brief    Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw, bool hasSensor);
    float ODOMETRY_LEFT_RIGHT_RADIUS;
    float ODOMETRY_BACK_RADIUS;
    const float ODOMETRY_CIRCUMFERENCE  = 3.25 * M_PI;
    const double WHEELCIRC = 3.25 * M_PI; // Drive train wheel circumference in inches --> CHECK: might be 3 inches!
    double auto_drive_velocity_percent = 20;
    double auto_rotate_velocity_percent = 20;
    double DRIVETRAINWIDTH; // distance (in inch) between left and right side of the drivetrain (measured from the center of the wheels)

private:
    Hardware* hw;
};