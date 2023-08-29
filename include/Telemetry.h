#pragma once
#include "vex.h"
#include "Hardware.h"
#include "RobotConfig.h"

/// @brief    Contains all the methods for calculation the robots position.
class Telemetry {
public:
    Telemetry(Hardware* hardware, RobotConfig* robotConfig);
private:
    Hardware* hw;
    RobotConfig* rc;    
};



