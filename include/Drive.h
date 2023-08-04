#pragma once
#include "vex.h"
#include "Hardware.h"
#include "Telemetry.h"
#include "RobotConfig.h"
#include "Logger.h"

class Drive{
public:
    virtual void drive(){}

protected:
    Drive(Hardware* hardware, RobotConfig* robotConfig, Telemetry* telemetry);

    Hardware* hw;
    RobotConfig* rc;
    Telemetry* tm;
};

