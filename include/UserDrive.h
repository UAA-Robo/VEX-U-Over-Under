#pragma once
#include "Drive.h"

class UserDrive : public Drive{
public:
    UserDrive(Hardware* hardware, RobotConfig* robotConfig, Telemetry* telemetry);
    void drive();
    void macroControls();
    void testPrint();

private:
    void getInputs(int32_t*); // Pass in macro_inputs[macro_loop_iteration] (even if macro is not running)

    int macro_loop_iteration;
    bool macro_running, macro_recording;
    int32_t forward_backward, left_right;
    bool button_L1, button_L2, button_R1, button_R2, button_A, button_B,
    button_X, button_Y, button_up, button_down, button_left, button_right;
    std::vector<int32_t> *macro_inputs = new std::vector<int32_t>;

};