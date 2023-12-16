#pragma once
#include "Hardware.h"

/// @brief    Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw);

    int DRIVETRAINWIDTH = 10.0; // ! Temporary!
private:
    Hardware* hw;

};
