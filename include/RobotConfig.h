#pragma once
#include "Hardware.h"

/// @brief    Stores any data specific to each robot.
class RobotConfig {
public:
    RobotConfig(Hardware* hw);

    int DRIVETRAINWIDTH = 10.0; //! Temporary!

    bool USE_PID_IN_USERDRIVE = false;
    bool USE_PID_IN_AUTODRIVE = false; //TODO: Change to true later

private:
    Hardware* hw;

};
