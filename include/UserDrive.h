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

    /// @brief Spins the catapult motors until limit switch is hit. Loops forever because it is on,
    ///     it's own thread.
    static int run_catapult(void* param);

    /// @brief Pushes air out when X button is pressed to expand snowplow.
    void snowplow_out();

    /// @brief Sucks air in when Y button is pressed to retract snowplow.
    void snowplow_in();

    /// @brief Activates intake motor while L1 is pressed.
    void activate_intake();

    /// @brief Expand intake motor while A is pressed. Retracts intake motor while B is pressed.
    void adjust_intake();

};