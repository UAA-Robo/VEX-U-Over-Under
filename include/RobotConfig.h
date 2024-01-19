#pragma once
#include "Hardware.h"

/// @brief Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw);
    float ODOMETRY_LEFT_RIGHT_RADIUS;
    float ODOMETRY_BACK_RADIUS;
    float ODOMETRY_BACK_OFFSET;
    float ODOMETRY_CIRCUMFERENCE;
    //double WHEELCIRC;
    double auto_drive_velocity_percent;
    double auto_rotate_velocity_percent;
    double DRIVETRAIN_WIDTH; // distance (in inch) between left and right side of the drivetrain (measured from the center of the wheels)
    double ACTUAL_WIDTH; // Largest width of the robot from the outside.

    // for MAP offsets (test)
    double DRIVETRAIN_RADIUS;
    double ACTUAL_RADIUS;

    double WHEEL_CIRCUMFERENCE;
    double CLIMB_WHEEL_DIAMETER = 3.0;

    double ODOMETRY_DIAMETER;

    robot_name ROBOT;
    

    // Starting position
    //! TODO: This is for SCRAT ONLY
    std::pair<double, double> starting_pos = {-53.23, 53.23}; // old is -53.23, 53.23
    double starting_heading = 135.0;

private:
    Hardware* hw;
};