#include "UserDrive.h"
#include "iostream"

UserDrive::UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) 
{
    macro_running = false;
    macro_recording = false;
    macro_length = -2;
    forward_backward.previous = 0;
    left_right.previous = 0;
    button_A.previous = false;
    button_B.previous = false;
    button_X.previous = false;
    button_Y.previous = false;
    button_L1.previous = false;
    button_L2.previous = false;
    button_R1.previous = false;
    button_R2.previous = false;
    button_up.previous = false;
    button_down.previous = false;
    button_left.previous = false;
    button_right.previous = false;
}

void UserDrive::drive()
{
    hw->controller.Screen.clearScreen();
    hw->controller.Screen.setCursor(1, 1);
    getInputs(); // Get inputs from controller or recorded macro
    macroControls(); // Run macro controls from user input
    testPrint(); // Prints when A, B, X, or Y are pressed on Brain Screen
    drivetrain_controls();




    setPreviousInputs(); // Tracks previous inputs to compare to
    if (macro_loop_iteration == macro_length) macro_running = false;
    if (macro_recording || macro_running) ++macro_loop_iteration; // Last item
}

void UserDrive::drivetrain_controls()
{
    const int DEADZONE = 2;

    if (std::abs(forward_backward.value) < DEADZONE)
    {
        forward_backward.value = 0;
    }

    if (std::abs(left_right.value) < DEADZONE)
    {
        left_right.value = 0;
    }

    move_drivetrain({forward_backward.value, left_right.value});
}


void UserDrive::getInputs()
{
    if (macro_running)
    {
        hw->controller.Screen.print("MACRO: RUNNING");
        forward_backward.value = macro_inputs[macro_loop_iteration][0];
        left_right.value = macro_inputs[macro_loop_iteration][1];
        button_L1.value = (bool)macro_inputs[macro_loop_iteration][2];
        button_L2.value = (bool)macro_inputs[macro_loop_iteration][3];
        button_R1.value = (bool)macro_inputs[macro_loop_iteration][4];
        button_R2.value = (bool)macro_inputs[macro_loop_iteration][5];
        button_A.value = (bool)macro_inputs[macro_loop_iteration][6];
        button_B.value = (bool)macro_inputs[macro_loop_iteration][7];
        button_X.value = (bool)macro_inputs[macro_loop_iteration][8];
        button_Y.value = (bool)macro_inputs[macro_loop_iteration][9];
        button_up.value = hw->controller.ButtonUp.pressing(); // Stop running macro early by user input
        button_down.value = false; // Cannot record a macro while running a macro!
        button_left.value = (bool)macro_inputs[macro_loop_iteration][12];
        button_right.value = (bool)macro_inputs[macro_loop_iteration][13];
    }
    else
    {
        if (macro_recording) {
            hw->controller.Screen.print("MACRO: RECORDING");
            macro_inputs[macro_loop_iteration].push_back(hw->controller.Axis3.position(vex::percent));
            macro_inputs[macro_loop_iteration].push_back(hw->controller.Axis1.position(vex::percent));
            macro_inputs[macro_loop_iteration].push_back((int)hw->controller.ButtonL1.pressing());
            macro_inputs[macro_loop_iteration].push_back((int)hw->controller.ButtonL2.pressing());
            macro_inputs[macro_loop_iteration].push_back((int)hw->controller.ButtonR1.pressing());
            macro_inputs[macro_loop_iteration].push_back((int)hw->controller.ButtonR2.pressing());
            macro_inputs[macro_loop_iteration].push_back((int)hw->controller.ButtonA.pressing());
            macro_inputs[macro_loop_iteration].push_back((int)hw->controller.ButtonB.pressing());
            macro_inputs[macro_loop_iteration].push_back((int)hw->controller.ButtonX.pressing());
            macro_inputs[macro_loop_iteration].push_back((int)hw->controller.ButtonY.pressing());
            macro_inputs[macro_loop_iteration].push_back(0); // Cannot run a macro while recording a macro!
            button_down.value = hw->controller.ButtonDown.pressing(); // Stop recording macro
            macro_inputs[macro_loop_iteration].push_back((int)hw->controller.ButtonLeft.pressing());
            macro_inputs[macro_loop_iteration].push_back((int)hw->controller.ButtonRight.pressing());
            macro_inputs.push_back(std::vector<int>());
        }
        forward_backward.value = hw->controller.Axis3.position(vex::percent);
        left_right.value = hw->controller.Axis1.position(vex::percent);
        button_L1.value = hw->controller.ButtonL1.pressing();
        button_L2.value = hw->controller.ButtonL2.pressing();
        button_R1.value = hw->controller.ButtonR1.pressing();
        button_R2.value = hw->controller.ButtonR2.pressing();
        button_A.value = hw->controller.ButtonA.pressing();
        button_B.value = hw->controller.ButtonB.pressing();
        button_X.value = hw->controller.ButtonX.pressing();
        button_Y.value = hw->controller.ButtonY.pressing();
        button_up.value = hw->controller.ButtonUp.pressing();
        button_down.value = hw->controller.ButtonDown.pressing();
        button_left.value = hw->controller.ButtonLeft.pressing();
        button_right.value = hw->controller.ButtonRight.pressing();
    }
}

void UserDrive::setPreviousInputs()
{
    forward_backward.previous = forward_backward.value;
    left_right.previous = left_right.value;
    button_A.previous = button_A.value;
    button_B.previous = button_B.value;
    button_X.previous = button_Y.value;
    button_L1.previous = button_L1.value;
    button_L2.previous = button_L2.value;
    button_R1.previous = button_R1.value;
    button_R2.previous = button_R2.value;
    button_up.previous = button_up.value;
    button_down.previous = button_down.value;
    button_left.previous = button_left.value;
    button_right.previous = button_right.value;
}

void UserDrive::macroControls()
{
    if (button_down.value == true && button_down.previous == false && !macro_running)
    {
        if (macro_recording) { // Stop recording macro
            hw->controller.Screen.print("MACRO: STOP RECORD");
            macro_length = macro_loop_iteration;
            macro_loop_iteration = -1;
            macro_recording = false;
        }
        else if (!macro_recording) { // Start recording macro
            hw->controller.Screen.print("MACRO: START RECORD");
            if (macro_inputs.size() != 0) macro_inputs.clear();
            macro_inputs.push_back(std::vector<int>());
            macro_loop_iteration = -1;
            macro_recording = true;
        }
    }
    if (button_up.value == true && button_up.previous == false && !macro_recording)
    {
        if (macro_running) { // Stop running macro
            hw->controller.Screen.print("MACRO: STOP RUN");
            macro_loop_iteration = -1;
            macro_running = false;
        }
        else if (!macro_running) { // Start running macro
            hw->controller.Screen.print("MACRO: START RUN");
            macro_loop_iteration = -1;
            macro_running = true;
        }
    }
    
}


void UserDrive::testPrint()
{
    hw->brain.Screen.clearScreen(0);
    if (button_A.value) hw->brain.Screen.printAt(10, 20, "%c", 'A');
    if (button_B.value) hw->brain.Screen.printAt(40, 20, "%c", 'B');
    if (button_X.value) hw->brain.Screen.printAt(10, 40, "%c", 'X');
    if (button_Y.value) hw->brain.Screen.printAt(40, 40, "%c", 'Y');
}