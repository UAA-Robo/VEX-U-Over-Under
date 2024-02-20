#include "UserDrive.h"
#include "iostream"

UserDrive::UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) 
    : Drive(hardware, robotConfig, telemetry) {}

void UserDrive::drive()
{

    hw->right_intake_expansion_motor.setBrake(vex::brakeType::hold);
    hw->left_intake_expansion_motor.setBrake(vex::brakeType::hold);
    // Expand intake
    intake_count = 0;
    expand_intake();
    if (rc->ROBOT == SCRAT)  vex::wait(500, vex::timeUnits::msec);
    vex::wait(500, vex::timeUnits::msec);
    stop_intake_expansion();

    // Then start catapult thread
    //if (rc->ROBOT == SCRATETTE) run_catapult_catapult_strategy(30);
    //else vex::task catapult_task = vex::task(run_catapult_thread, this, 1);
    vex::task catapult_task = vex::task(run_catapult_thread, this, 1);


    while(true) {
        activate_catapult_strategy();
        drivetrain_controls();
        catapult_controls();
        intake_controls();
        snowplow_controls();

        vex::wait(20, vex::msec);  // Wait necessary to give time to other threads
    }
}

void UserDrive::drivetrain_controls() {
    const int DEADZONE = 2;
    float forward_backward = hw->controller.Axis3.position(vex::percentUnits::pct);
    float left_right = hw->controller.Axis1.position(vex::percentUnits::pct);

    if (std::abs(forward_backward) < DEADZONE) {
        forward_backward = 0;
    }

    if (std::abs(left_right) < DEADZONE) {
        left_right = 0;
    }

    move_drivetrain({forward_backward, left_right});
}



void UserDrive::snowplow_controls() {
    if (hw->controller.ButtonX.pressing()) {
        snowplow_out();
    }else if (hw->controller.ButtonY.pressing()) {
        snowplow_in();
    }
}


void UserDrive::catapult_controls()
{
    if (hw->controller.ButtonL1.pressing()) start_catapult();
    else stop_catapult();
}


void UserDrive::intake_controls()
{
    if (hw->controller.ButtonR1.pressing()) {
        if (!INTAKE_EXPANDED) {
            expand_intake();
            activate_intake();    
        }
        INTAKE_EXPANDED = true;
        intake_count = 0;
    } else if (!hw->controller.ButtonR2.pressing() && !hw->controller.ButtonL2.pressing()) {
        if (INTAKE_EXPANDED) {
            retract_intake();
            stop_intake();
        }
        INTAKE_EXPANDED = false;
        intake_count = 0;
    } else if (hw->controller.ButtonR2.pressing()) {
        if (!INTAKE_EXPANDED) {
            expand_intake();
            hw->right_intake_motor.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
        }
        INTAKE_EXPANDED = true;
        intake_count = 0;
    } else if (hw->controller.ButtonL2.pressing()) {
        if (INTAKE_EXPANDED) {
            retract_intake();
            hw->right_intake_motor.stop();
        }
    }
    if (intake_count >= 800) stop_intake_expansion();
    intake_count += 20;

    // // Activate intake when expanded
    // if (hw->controller.ButtonB.pressing()) {
    //     reverse_intake();
    // }
    // else if (INTAKE_EXPANDED && !hw->controller.ButtonL1.pressing()) {
    //     activate_intake();  
    // } 
    // else {
    //     stop_intake();
    // }



}

void UserDrive::activate_catapult_strategy()
{

    if (hw->controller.ButtonR2.pressing()) { 
        if (!CATAPULT_STRATEGY_RAN) {
            // Expand intake
            expand_intake();
            vex::wait(500, vex::timeUnits::msec);
            stop_intake_expansion();
        }
            run_catapult_once(); 
        CATAPULT_STRATEGY_RAN = true;
    }else {
        CATAPULT_STRATEGY_RAN = false;
    }

}
