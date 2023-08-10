#include "UserDrive.h"

UserDrive::UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) 
{
    macro_running = false;
    macro_recording = false;
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

    // std::cout << hw->controller.Axis1.position(vex::percentUnits::pct) << " "
    //           << hw->controller.Axis3.position(vex::percentUnits::pct)<< std::endl;
    //std::cout << forward_backward << " " << turning << std::endl;

    move_drivetrain({forward_backward, turning});
    getInputs(macro_inputs[macro_loop_iteration]) // Get inputs from controller or recorded macro
    macroControls(); // Run macro controls from user input
    testPrint(); // Prints when A, B, X, or Y are pressed on Brain Screen
    /*
        Call all user drive motor functions 
        (private variables available for all inputs)
    
    */

    if (macro_recording || macro_running) ++macro_loop_iteration;
}


void UserDrive::getInputs(int32_t* inputs)
{
    if (macro_running)
    {
        forward_backward = inputs[0];
        left_right = inputs[1];
        button_L1 = (bool)inputs[2];
        button_L2 = (bool)inputs[3];
        button_R1 = (bool)inputs[4];
        button_R2 = (bool)inputs[5];
        button_A = (bool)inputs[6];
        button_B = (bool)inputs[7];
        button_X = (bool)inputs[8];
        button_Y = (bool)inputs[9];
        button_up = hw->controller.ButtonUp.pressing(); // Stop running macro early by user input
        button_down = false; // Cannot record a macro while running a macro!
        button_left = (bool)inputs[12];
        button_right = (bool)inputs[13];
    }
    else if (macro_recording)
    {
        int32_t inputs[16];
        inputs[0] = hw->controller.Axis3.position(vex::percent);
        inputs[1] = hw->controller.Axis1.position(vex::percent);
        inputs[2] = (int32_t)hw->controller.ButtonL1.pressing();
        inputs[3] = (int32_t)hw->controller.ButtonL2.pressing();
        inputs[4] = (int32_t)hw->controller.ButtonR1.pressing();
        inputs[5] = (int32_t)hw->controller.ButtonR2.pressing();
        inputs[6] = (int32_t)hw->controller.ButtonA.pressing();
        inputs[7] = (int32_t)hw->controller.ButtonB.pressing();
        inputs[8] = (int32_t)hw->controller.ButtonX.pressing();
        inputs[9] = (int32_t)hw->controller.ButtonY.pressing();
        inputs[10] = 0; // Cannot run a macro while recording a macro!
        button_down = (int32_t)hw->controller.ButtonDown.pressing(); // Stop recording macro
        inputs[12] = (int32_t)hw->controller.ButtonLeft.pressing();
        inputs[13] = (int32_t)hw->controller.ButtonRight.pressing();

        macro_inputs.push_back(inputs);
    }
    else
    {
        forward_backward = hw->controller.Axis3.position(vex::percent);
        left_right = hw->controller.Axis1.position(vex::percent);
        button_L1 = hw->controller.ButtonL1.pressing();
        button_L2 = hw->controller.ButtonL2.pressing();
        button_R1 = hw->controller.ButtonR1.pressing();
        button_R2 = hw->controller.ButtonR2.pressing();
        button_A = hw->controller.ButtonA.pressing();
        button_B = hw->controller.ButtonB.pressing();
        button_X = hw->controller.ButtonX.pressing();
        button_Y = hw->controller.ButtonY.pressing();
        button_up = hw->controller.ButtonUp.pressing();
        button_down = hw->controller.ButtonDown.pressing();
        button_left = hw->controller.ButtonLeft.pressing();
        button_right = hw->controller.ButtonRight.pressing();
    }
}

void UserDrive::macroControls()
{
    if (button_down && !macro_recording && !macro_running) // Start recording macro
    {
        macro_inputs.clear();
        macro_loop_iteration = 0;
        macro_recording = true;
    }
    if (button_down && macro_recording) // Stop recording macro
    {
        macro_loop_iteration = 0;
        macro_recording = false;
    }
    if (button_up && !macro_running) // Start running macro
    {
        macro_loop_iteration = 0;
        macro_running = true;
    }
    if (button_up && macro_running) // Stop running macro
    {
        macro_loop_iteration = 0;
        macro_running = true;
    }
}


void UserDrive::testPrint()
{
    if (button_A) hw->brain.Screen.printAt(10, 20, 'A');
    if (button_B) hw->brain.Screen.printAt(40, 20, 'B');
    if (button_X) hw->brain.Screen.printAt(10, 40, 'X');
    if (button_Y) hw->brain.Screen.printAt(40, 40, 'Y');
}