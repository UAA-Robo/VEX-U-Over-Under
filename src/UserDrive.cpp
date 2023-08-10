#include "UserDrive.h"

UserDrive::UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry)
{
}

void UserDrive::drive()
{
    drivetrain_controls();
}

void UserDrive::drivetrain_controls()
{
    const int DEADZONE = 2;

    double forward_backward = (double)hw->controller.Axis3.position(vex::percentUnits::pct);
    double turning = (double)hw->controller.Axis1.position(vex::percentUnits::pct);

    if (std::abs(forward_backward) < DEADZONE)
    {
        forward_backward = 0;
    }

    if (std::abs(turning) < DEADZONE)
    {
        turning = 0;
    }

    move_drivetrain({forward_backward, turning});
}