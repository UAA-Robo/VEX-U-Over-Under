#pragma once
#include "Drive.h"

class UserDrive : public Drive
{
public:
    UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry);
    void drive();

private:
    /// @brief  Controls drivetrain based on controller joysticks. Up/down on the left joystick is forward/backward.
    //          Left/right on the right joystick is turning.
    void drivetrain_controls();
};