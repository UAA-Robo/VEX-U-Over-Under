#pragma once
#include "Hardware.h"

/// @brief    Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw);

    int DRIVETRAINWIDTH = 14.0; //! Temporary, configure actual values later

private:
    Hardware* hw;

};
