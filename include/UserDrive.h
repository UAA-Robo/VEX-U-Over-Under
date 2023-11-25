#pragma once
#include "Drive.h"

/// @brief   Contains the methods for the robot to be controlled by drivers.
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

    bool CATAPULT_RETURNING_TO_POSITION = true;
    int last_catapult_limit_switch_value = 1;

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

    /// @brief Test action function TO BE REMOVED LATER
    void test_print();

    /// @brief For testing pneumatics. Pushes air out when left button is pressed.
    void pneumatic_out();

    /// @brief For testing pneumatics. Sucks air in when right button is pressed.
    void pneumatic_in();

    /// @brief Spins catapult motors when right bumer (R1) is pressed.
    void launch_catapult();

    /// @brief Auto-return catapult to position.
    void return_catapult_to_position();

    /// @brief For testing purposes. Activates intake motor while left button (<) is pressed.
    void activate_intake();

    /// @brief For testing purposes. Retracts intake motor while right button (>) is pressed.
    void retract_intake();

    void limit_switch_pressed();
};