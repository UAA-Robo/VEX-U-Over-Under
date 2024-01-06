#include "UserDrive.h"
#include "iostream"

UserDrive::UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) 
    : Drive(hardware, robotConfig, telemetry), tick(0) {
    IS_MACRO_RUNNING = false;
    IS_MACRO_RECORDING = false;
    macro_length = -2;
    
    
    if (input_list.size() == 0) // Setup vector
    {
        input_list.reserve(14);
        controller_values.reserve(14);

        for (int i = 0; i < 14; ++i)
        {
            input_list.push_back(0);
            controller_values.push_back(0);
        }
    }
    // List of inputs by address
    input_list[0] = &forward_backward;
    input_list[1] = &left_right;
    input_list[2] = &button_L1;
    input_list[3] = &button_L2;
    input_list[4] = &button_R1;
    input_list[5] = &button_R2;
    input_list[6] = &button_A;
    input_list[7] = &button_B;
    input_list[8] = &button_X;
    input_list[9] = &button_Y;
    input_list[10] = &button_up;
    input_list[11] = &button_down;
    input_list[12] = &button_left;
    input_list[13] = &button_right;

    for (int i = 0; i < input_list.size(); ++i) input_list[i]->previous = 0;


}

void UserDrive::drive()
{

    while(true) {
        get_inputs();
        macro_controls();
        drivetrain_controls();
        snowplow_in();
        snowplow_out();
        activate_intake();
        adjust_intake();


        set_previous_inputs(); // Tracks previous inputs to compare to
        if (macro_loop_iteration == macro_length) IS_MACRO_RUNNING = false;
        if (IS_MACRO_RECORDING || IS_MACRO_RUNNING) ++macro_loop_iteration; // Last item
        vex::wait(20, vex::msec); 
    }
}

void UserDrive::drivetrain_controls() {
    const int DEADZONE = 2;

    if (std::abs(forward_backward.value) < DEADZONE) {
        forward_backward.value = 0;
    }

    if (std::abs(left_right.value) < DEADZONE) {
        left_right.value = 0;
    }
    forward_backward.value = forward_backward.value / (M_PI / 2) * 100;

    move_drivetrain({forward_backward.value, left_right.value});
}

void UserDrive::get_inputs() {

    // List of controller values
    controller_values[0] = hw->controller.Axis3.position(vex::percentUnits::pct);
    controller_values[1] = hw->controller.Axis1.position(vex::percentUnits::pct);
    controller_values[2] = (int32_t)hw->controller.ButtonL1.pressing();
    controller_values[3] = (int32_t)hw->controller.ButtonL2.pressing();
    controller_values[4] = (int32_t)hw->controller.ButtonR1.pressing();
    controller_values[5] = (int32_t)hw->controller.ButtonR2.pressing();
    controller_values[6] = (int32_t)hw->controller.ButtonA.pressing();
    controller_values[7] = (int32_t)hw->controller.ButtonB.pressing();
    controller_values[8] = (int32_t)hw->controller.ButtonX.pressing();
    controller_values[9] = (int32_t)hw->controller.ButtonY.pressing();
    controller_values[10] = (int32_t)hw->controller.ButtonUp.pressing();
    controller_values[11] = (int32_t)hw->controller.ButtonDown.pressing();
    controller_values[12] = (int32_t)hw->controller.ButtonLeft.pressing();
    controller_values[13] = (int32_t)hw->controller.ButtonRight.pressing();

    if (IS_MACRO_RUNNING) {
        hw->controller.Screen.print("MACRO: RUNNING");
        for (int i = 0; i < input_list.size(); ++i) {
            input_list[i]->value = macro_inputs[macro_loop_iteration][i];
        }
        button_up.value = hw->controller.ButtonUp.pressing(); // Stop running macro early by user
                                                              // input
        button_down.value = false;                            // Cannot record a macro while running a macro!
    }
    else
        for (int i = 0; i < input_list.size(); ++i)
            input_list[i]->value = controller_values[i];
    if (IS_MACRO_RECORDING)
    {
        hw->controller.Screen.print("MACRO: RECORDING");
        for (int i = 0; i < input_list.size(); ++i)
            macro_inputs[macro_loop_iteration].push_back(controller_values[i]);
        macro_inputs[macro_loop_iteration][10] = 0;
        button_down.value = hw->controller.ButtonDown.pressing();
        macro_inputs.push_back(std::vector<int>());
    }
}

void UserDrive::set_previous_inputs()
{
    for (int i = 0; i < input_list.size(); ++i)
        input_list[i]->previous = input_list[i]->value;
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
            if (macro_inputs.size() != 0)
                macro_inputs.clear();
            macro_inputs.push_back(std::vector<int>());
            macro_loop_iteration = -1;
            IS_MACRO_RECORDING = true;
        }
    }
    if (button_up.value && !button_up.previous && !IS_MACRO_RECORDING) {
        if (IS_MACRO_RUNNING) { // Stop running macro
            hw->controller.Screen.print("MACRO: STOP RUN");
            macro_loop_iteration = -1;
            IS_MACRO_RUNNING = false;
        } else if (!IS_MACRO_RUNNING) { // Start running macro
            hw->controller.Screen.print("MACRO: START RUN");
            macro_loop_iteration = -1;
            IS_MACRO_RUNNING = true;
        }
    }
}


void UserDrive::snowplow_out() {
    if (button_X.value) {
        hw->right_plow.set(false);
        hw->left_plow.set(false);
    }

}

void UserDrive::snowplow_in() {

    if (button_Y.value) {
        hw->right_plow.set(true);
        hw->left_plow.set(true);
    }

}

// int UserDrive::run_catapult(void* param)
// {
//     // WARNING: DON'T print in this thread or it will take too long and miss the catapult press
//     UserDrive* ud = static_cast<UserDrive*>(param);
    
//     while(1) {
        
//         if (ud->hw->catapult_limit_switch.value() == 0) {
//             ud->hw->catapult.stop();
//             ud->CATAPULT_STOPPED = true; // if limit switch touched, stop catapult
//         }
//         if (ud->button_R1.value == 1) ud->CATAPULT_STOPPED = false;
//         if (!ud->CATAPULT_STOPPED) {
//             ud->hw->catapult.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
//         }
//         vex::wait(10, vex::timeUnits::msec);
//     }
// }

void UserDrive::activate_intake()
{
    if (button_L1.value == 1) {
        hw->intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
        hw->controller.Screen.setCursor(1, 1);
        hw->controller.Screen.print("Activating Intake!");
    } else {
        hw->intake.stop();
    }
}


void UserDrive::adjust_intake()
{
    if (button_A.value == 1) {
        hw->intake_expansion.spin(vex::directionType::rev, 6, vex::voltageUnits::volt);
        hw->controller.Screen.setCursor(1, 1);
        hw->controller.Screen.print("Expanding Intake!");
    } else if (button_B.value == 1) {
        hw->intake_expansion.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);
        hw->controller.Screen.setCursor(1, 1);
        hw->controller.Screen.print("Retracting Intake!");
    } else {
        hw->intake_expansion.stop();
    }
}
