#pragma once
#include "vex.h"
#include "Hardware.h"
#include "RobotConfig.h"

class Telemetry {
public:
    Telemetry(Hardware* hardware, RobotConfig* robotConfig);
private:
    Hardware* hw;
    RobotConfig* rc;    
};



