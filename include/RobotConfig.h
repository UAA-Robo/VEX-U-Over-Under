#pragma once
#include "Hardware.h"

/// @brief    Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw);
    const float ODOMETRY_LEFT_RIGHT_RADIUS = 2; // TODO: change
    const float ODOMETRY_BACK_RADIUS = 10;  // TODO: change
    const float ODOMETRY_CIRCUMFERENCE  = 2.75 * M_PI;
private:
    Hardware* hw;

};
