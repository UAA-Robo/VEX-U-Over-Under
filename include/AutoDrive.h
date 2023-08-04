#pragma once
#include "Drive.h"
#include "vex.h"
class AutoDrive : public Drive
{
public:
    AutoDrive(Hardware* hardware, RobotConfig* robotConfig, Telemetry* telemetry);
    void drive();

};