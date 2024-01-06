#pragma once
#include "Drive.h"
#include "PathGenerator.h"
#include "vex.h"

/// @brief   Contains the methods for the robot to autonomously navigate.
class AutoDrive : public Drive {

public:
    AutoDrive(Hardware* hardware, RobotConfig* robotConfig, Telemetry* telemetry);

    /// @brief    Main function that initiates to robot.
    void drive();

private:
    PathGenerator *pg;
    Map *mp;

};