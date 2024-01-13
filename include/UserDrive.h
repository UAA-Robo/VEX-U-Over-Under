#pragma once
#include "Drive.h"

/// @brief Contains the methods for the robot to be controlled by drivers.
class UserDrive : public Drive {
public:
    UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry);
    void drive();
    

    

private:
    struct input {
    int value;
    int previous;

    };

    int tick;

    bool CATAPULT_RUNNING = false;
    bool CATAPULT_STOPPED = false;

    std::vector<input*> input_list;
    std::vector<int32_t> controller_values;

    int macro_loop_iteration;
    int macro_length;
    bool IS_MACRO_RUNNING, IS_MACRO_RECORDING;
    input forward_backward, left_right, button_L1, button_L2, button_R1, button_R2, button_A,
    button_B, button_X, button_Y, button_up, button_down, button_left, button_right;
    std::vector<std::vector<int>> macro_inputs;

    /// @brief Controls for recording and running macros
    void macro_controls();

    /// @brief Stores inputs from either controller or recorded macro to variables accessible by
    ///        action functions
    void get_inputs();

    /// @brief Function to record all current inputs to the set of previous inputs for comparison
    void set_previous_inputs();


    /// @brief  Controls drivetrain based on controller joysticks. Up/down on the left joystick is forward/backward.
    //          Left/right on the right joystick is turning.
    void drivetrain_controls();

    /// @brief Launches the catapult when R1 is pressed. Resets to down (limit switch otherwise)
    void catapult_controls();

    /// @brief Controls the snowplow. X button expands the plows out and Y retracts them in.
    void snowplow_controls();

    /// @brief Controls the intake. A button expands the intake out and Y retracts them in.
    void intake_controls();

    bool INTAKE_EXPANDED = false;



};