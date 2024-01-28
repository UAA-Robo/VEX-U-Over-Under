#include "UserDrive.h"
#include "iostream"

UserDrive::UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) 
    : Drive(hardware, robotConfig, telemetry), tick(0) {
    
    
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
    // Expand intake
    expand_intake();
    if (rc->ROBOT == SCRAT)  vex::wait(500, vex::timeUnits::msec);
    vex::wait(500, vex::timeUnits::msec);
    stop_intake_expansion();

    // Then start catapult thread
    //if (rc->ROBOT == SCRATETTE) run_catapult_catapult_strategy(30);
    //else vex::task catapult_task = vex::task(run_catapult_thread, this, 1);
    vex::task catapult_task = vex::task(run_catapult_thread, this, 1);


    while(true) {
        get_inputs();
        drivetrain_controls();
        catapult_controls();
        intake_controls();
        snowplow_controls();

        set_previous_inputs(); // Tracks previous inputs to compare to
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
    //forward_backward.value = forward_backward.value / (M_PI / 2) * 100;

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
}

void UserDrive::set_previous_inputs()
{
    for (int i = 0; i < input_list.size(); ++i)
        input_list[i]->previous = input_list[i]->value;
}


void UserDrive::snowplow_controls() {
    if (button_X.value == 1) {
        snowplow_out();
    }else if (button_Y.value == 1) {
        snowplow_in();
    }
}


void UserDrive::catapult_controls()
{
    if (button_R1.value == 1) start_catapult();
    else stop_catapult();
}


void UserDrive::intake_controls()
{
    if (button_L1.value == 1){
        expand_intake();
        INTAKE_EXPANDED = true;
    } else if (button_L2.value == 1) {
        retract_intake();
        INTAKE_EXPANDED = false;
    } else {
        stop_intake_expansion();
    }

    // Activate intake when expanded
    if (INTAKE_EXPANDED && button_L1.value == 0) {
        activate_intake();
    }else {
        stop_intake();
    }
}
