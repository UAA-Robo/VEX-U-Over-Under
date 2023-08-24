#include "UserDrive.h"
#include "iostream"

UserDrive::UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) 
{
    IS_MACRO_RUNNING = false;
    IS_MACRO_RECORDING = false;
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
    get_inputs();
    macro_controls();
    test_print();
    drivetrain_controls();




    set_previous_inputs(); // Tracks previous inputs to compare to
    if (macro_loop_iteration == macro_length) IS_MACRO_RUNNING = false;
    if (IS_MACRO_RECORDING || IS_MACRO_RUNNING) ++macro_loop_iteration; // Last item
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


void UserDrive::get_inputs()
{
    std::vector<int*> input_list;
    std::vector<int*> controller_values;
    // List of inputs by address
    input_list[0] = &forward_backward.value;
    input_list[1] = &left_right.value;
    input_list[2] = &button_L1.value;
    input_list[3] = &button_L2.value;
    input_list[4] = &button_R1.value;
    input_list[5] = &button_R2.value;
    input_list[6] = &button_A.value;
    input_list[7] = &button_B.value;
    input_list[8] = &button_X.value;
    input_list[9] = &button_Y.value;
    input_list[10] = &button_up.value;
    input_list[11] = &button_down.value;
    input_list[12] = &button_left.value;
    input_list[13] = &button_right.value;
    // List of controller values
    controller_values[0] = hw->controller.Axis3.position(vex::percentUnits::pct);
    controller_values[1] = hw->controller.Axis1.position(vex::percentUnits::pct);
    controller_values[2] = hw->controller.ButtonL1.pressing();
    controller_values[3] = hw->controller.ButtonL2.pressing();
    controller_values[4] = hw->controller.ButtonR1.pressing();
    controller_values[5] = hw->controller.ButtonR2.pressing();
    controller_values[6] = hw->controller.ButtonA.pressing();
    controller_values[7] = hw->controller.ButtonB.pressing();
    controller_values[8] = hw->controller.ButtonX.pressing();
    controller_values[9] = hw->controller.ButtonY.pressing();
    controller_values[10] = hw->controller.ButtonUp.pressing();
    controller_values[11] = hw->controller.ButtonDown.pressing();
    controller_values[12] = hw->controller.ButtonLeft.pressing();
    controller_values[13] = hw->controller.ButtonRight.pressing();

    if (IS_MACRO_RUNNING)
    {
        hw->controller.Screen.print("MACRO: RUNNING");
        for (int i = 0; i < input_list.size(); ++i) *input_list[i] = macro_inputs[macro_loop_iteration][i];
        button_up.value = hw->controller.ButtonUp.pressing(); // Stop running macro early by user input
        button_down.value = false; // Cannot record a macro while running a macro!
    }
    else for (int i = 0; i < input_list.size(); ++i) *input_list[i] = *controller_values[i];
    if (IS_MACRO_RECORDING) {
        hw->controller.Screen.print("MACRO: RECORDING");
        for (int i = 0; i < input_list.size(); ++i) macro_inputs[macro_loop_iteration][i].push_back(*controller_values[i]);
        macro_inputs[macro_loop_iteration][10] = 0;
        button_down.value = hw->controller.ButtonDown.pressing();
        macro_inputs.push_back(std::vector<int>());
    }
}


void UserDrive::set_previous_inputs()
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



void UserDrive::macro_controls()
{
    if (button_down.value && !button_down.previous && !IS_MACRO_RUNNING)
    {
        if (IS_MACRO_RECORDING) { // Stop recording macro
            hw->controller.Screen.print("MACRO: STOP RECORD");
            macro_length = macro_loop_iteration;
            macro_loop_iteration = -1;
            IS_MACRO_RECORDING = false;
        }
        else if (!IS_MACRO_RECORDING) { // Start recording macro
            hw->controller.Screen.print("MACRO: START RECORD");
            if (macro_inputs.size() != 0) macro_inputs.clear();
            macro_inputs.push_back(std::vector<int>());
            macro_loop_iteration = -1;
            IS_MACRO_RECORDING = true;
        }
    }
    if (button_up.value && !button_up.previous && !IS_MACRO_RECORDING)
    {
        if (IS_MACRO_RUNNING) { // Stop running macro
            hw->controller.Screen.print("MACRO: STOP RUN");
            macro_loop_iteration = -1;
            IS_MACRO_RUNNING = false;
        }
        else if (!IS_MACRO_RUNNING) { // Start running macro
            hw->controller.Screen.print("MACRO: START RUN");
            macro_loop_iteration = -1;
            IS_MACRO_RUNNING = true;
        }
    }
    
}


void UserDrive::test_print()
{
    hw->brain.Screen.clearScreen(0);
    if (button_A.value) hw->brain.Screen.printAt(10, 20, "%c", 'A');
    if (button_B.value) hw->brain.Screen.printAt(40, 20, "%c", 'B');
    if (button_X.value) hw->brain.Screen.printAt(10, 40, "%c", 'X');
    if (button_Y.value) hw->brain.Screen.printAt(40, 40, "%c", 'Y');
}