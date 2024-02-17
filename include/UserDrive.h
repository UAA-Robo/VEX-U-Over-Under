#pragma once
#include "Drive.h"

/// @brief Contains the methods for the robot to be controlled by drivers.
class UserDrive : public Drive {
public:
    UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry);
    void drive();
    
private:
    bool CATAPULT_RUNNING = false;
    bool CATAPULT_STOPPED = false;

    /// @brief  Controls drivetrain based on controller joysticks. Up/down on the left joystick is forward/backward.
    //          Left/right on the right joystick is turning.
    void drivetrain_controls();

    /// @brief Launches the catapult when R1 is pressed. Resets to down (limit switch otherwise)
    void catapult_controls();

    /// @brief Controls the snowplow. X button expands the plows out and Y retracts them in.
    void snowplow_controls();

    /// @brief Controls the intake. A button expands the intake out and Y retracts them in.
    void intake_controls();

    /// @brief Turns on/off catapult strategy when R2 is pressed.
    void activate_catapult_strategy();

    /// @brief Press A button to activate climb motors
    void climb_controls();

    bool INTAKE_EXPANDED = false;
    bool CATAPULT_STRATEGY_RAN = false;
};