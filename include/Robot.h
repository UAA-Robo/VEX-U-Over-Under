#pragma once
#include "AutoDrive.h"
#include "UserDrive.h"
#include "Telemetry.h"
#include "Hardware.h"
#include "RobotConfig.h"
#include "RaspiComm.hpp"

#include "vex.h"

/// @brief   Holds all the classes needed to run the robot.
class Robot {

public:
    Robot();

    /// @brief    Initializes the driver-controlled code.
    void drive();

    /// @brief    Initializes the autonomous-period code.
    void driveAuto();
    Hardware* hw;
private:
    //Hardware* hw;
    RobotConfig* rc;
    Telemetry* tm;

    UserDrive* userDrive;
    AutoDrive* autoDrive;
};