#pragma once
#include "Hardware.h"

/// @brief Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw);
    float ODOMETRY_LEFT_RIGHT_RADIUS;
    float ODOMETRY_BACK_RADIUS;
    float ODOMETRY_CIRCUMFERENCE;
    //double WHEELCIRC;
    double auto_drive_velocity_percent;
    double auto_rotate_velocity_percent;
    double DRIVETRAIN_WIDTH; // distance (in inch) between left and right side of the drivetrain (measured from the center of the wheels)
    
    // for MAP offsets (test)
    double DRIVETRAIN_RADIUS = DRIVETRAIN_WIDTH * sqrt(2) / 2.0; //! Temporary, configure actual values later

    double ODOMETRY_DIAMETER;

    robot_name ROBOT;
    

    // Starting position
    std::pair<double, double> starting_pos = {-53.23, 53.23};
    double starting_heading = 315.0;

private:
    Hardware* hw;
};