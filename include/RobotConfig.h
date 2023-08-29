#pragma once
#include "Hardware.h"

/// @brief    Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw);

private:
    Hardware* hw;

};
