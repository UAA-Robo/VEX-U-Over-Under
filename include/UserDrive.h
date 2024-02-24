#pragma once
#include "Drive.h"

/// @brief Contains the methods for the robot to be controlled by drivers.
class UserDrive : public Drive {
public:
    UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry);
    void drive();
    
private:

    vex::task catapult_task;

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

    // /// @brief Press A button to activate climb motors
    // void climb_controls();

    /// @brief Reconnect to the catapult thread when we disconnect.
    void last_chance();

    


    const float LOW_DRIVETRAIN_VELOCITY = rc->IN_PLACE_LEFT_RIGHT_MULTIPLIER;
    const float HIGH_DRIVETRAIN_VELOCITY = rc->IN_PLACE_LEFT_RIGHT_MULTIPLIER;

    float fwd_bwd_joystick_multiplier = 1.0;
    float left_right_joystick_multiplier = HIGH_DRIVETRAIN_VELOCITY;

    bool CATAPULT_DISABLED = false;
    bool INTAKE_EXPANDED = false;
    bool INTAKE_HELD = false;
    bool INTAKE_IS_REVERSING = false;
    bool PLOW_EXPANDED = false;
    int plow_count;
    int intake_count;
    bool CATAPULT_STRATEGY_RAN = false;
};