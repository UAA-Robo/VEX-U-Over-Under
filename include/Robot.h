#pragma once
#include "AutoDrive.h"
#include "UserDrive.h"
#include "Telemetry.h"
#include "Hardware.h"
#include "RobotConfig.h"

#include "vex.h"


class Robot{
public:
    Robot();
    void drive();
    void driveAuto();
private:
    Hardware* hw;
    RobotConfig* rc;
    Telemetry* tm;

    UserDrive* userDrive;
    AutoDrive* autoDrive;
};