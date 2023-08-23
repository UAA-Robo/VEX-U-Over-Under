#pragma once
#include "Drive.h"


struct input_int {
    int value;
    int previous;
};
struct input_bool {
    bool value;
    bool previous;
};

class UserDrive : public Drive
{
public:
    UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry);
    void drive();
    /// @brief Controls for recording and running macros
    void macroControls();
    /// @brief Function to test macro feature by printing key presses (A, B, X, Y)
    void testPrint();

private:
    void getInputs();
    void setPreviousInputs();

    int macro_loop_iteration;
    int macro_length;
    bool IS_MACRO_RUNNING, IS_MACRO_RECORDING;
    input_int forward_backward, left_right;
    input_bool button_L1, button_L2, button_R1, button_R2, button_A, button_B,
    button_X, button_Y, button_up, button_down, button_left, button_right;
    //bool button_down_previous, button_up_previous;
    std::vector<std::vector<int>> macro_inputs;

    /// @brief  Controls drivetrain based on controller joysticks. Up/down on the left joystick is forward/backward.
    //          Left/right on the right joystick is turning.
    void drivetrain_controls();
};