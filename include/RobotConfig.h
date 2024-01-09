#pragma once
#include "Hardware.h"

/// @brief Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw, bool hasSensor);
    float ODOMETRY_LEFT_RIGHT_RADIUS;
    float ODOMETRY_BACK_RADIUS;
    float ODOMETRY_CIRCUMFERENCE;
    //double WHEELCIRC;
    double auto_drive_velocity_percent;
    double auto_rotate_velocity_percent;
    double DRIVETRAIN_WIDTH; // distance (in inch) between left and right side of the drivetrain (measured from the center of the wheels)

    int DRIVETRAINWIDTH = 14.0; //! Temporary, configure actual values later

    // Starting position
    std::pair<double, double> starting_pos = {-12.0, 0.0};
    double starting_heading = 0.0;

private:
    Hardware* hw;
};