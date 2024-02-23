#include "UserDrive.h"
#include "iostream"

UserDrive::UserDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) 
    : Drive(hardware, robotConfig, telemetry) {}

void UserDrive::drive()
{

    hw->right_intake_expansion_motor.setBrake(vex::brakeType::brake);
    hw->left_intake_expansion_motor.setBrake(vex::brakeType::brake);
    // Expand intake
    intake_count = 0;
    expand_intake();
    vex::wait(300, vex::timeUnits::msec);
    // Start catapult
    catapult_task = vex::task(run_catapult_thread, this, 2);
    vex::wait(300, vex::timeUnits::msec);
    stop_intake_expansion();
    INTAKE_EXPANDED = true;


    // Then start catapult thread
    //if (rc->ROBOT == SCRATETTE) run_catapult_catapult_strategy(30);
    //else vex::task catapult_task = vex::task(run_catapult_thread, this, 1);
    

    while(true) {

        activate_catapult_strategy();
        drivetrain_controls();
        catapult_controls();
        intake_controls();
        snowplow_controls();
        climb_controls();
        last_chance();

        snowplow_timeout += 20;
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

    // Keep responsive arcing
    if (forward_backward != 0 && left_right != 0 && !INTAKE_EXPANDED && !INTAKE_HELD) {
        left_right /= left_right_joystick_multiplier;
        left_right *= rc->ARCING_LEFT_RIGHT_MULTIPLIER;
        forward_backward *= rc->ARCING_FORWARD_BACKWARD_MULTIPLIER;
    }
    

    move_drivetrain(
        {
            forward_backward * fwd_bwd_joystick_multiplier,
            left_right * left_right_joystick_multiplier
        }
    );
}



void UserDrive::snowplow_controls() {
    if (hw->controller.ButtonA.pressing()) {
        if (!PLOW_EXPANDED) {
            snowplow_out();
            PLOW_EXPANDED = true;
        } else {
            snowplow_in();
            PLOW_EXPANDED = false;
        }
        plow_count += 20;
    }
}


void UserDrive::catapult_controls()
{
    if (rc->ROBOT == SCRATETTE && hw->controller.ButtonA.pressing()) release_catapult();
    else if (hw->controller.ButtonL1.pressing() && !CATAPULT_DISABLED) start_catapult();
    else stop_catapult();

    if (!hw->controller.ButtonA.pressing()) engage_catapult();


}


void UserDrive::intake_controls()
{
    if (hw->controller.ButtonX.pressing()) {
        // Stop intake
        stop_intake();
        left_right_joystick_multiplier = HIGH_DRIVETRAIN_VELOCITY;
    } else if (hw->controller.ButtonR1.pressing()) {
        // Expand while held and start intake
        if (!INTAKE_EXPANDED || INTAKE_HELD) {
            INTAKE_HELD = false;
            expand_intake();
            activate_intake();
            CATAPULT_DISABLED = false;
            left_right_joystick_multiplier = LOW_DRIVETRAIN_VELOCITY;
            intake_count = 0;    
        }
        INTAKE_EXPANDED = true;
        
    } else if (!hw->controller.ButtonR2.pressing() && !hw->controller.ButtonL2.pressing()
    && !INTAKE_HELD && !INTAKE_IS_REVERSING) {
        if (INTAKE_EXPANDED) {
            retract_intake();
            stop_intake();
            CATAPULT_DISABLED = true;
            left_right_joystick_multiplier = HIGH_DRIVETRAIN_VELOCITY;
            INTAKE_HELD = false;
            intake_count = 0;
        }
        INTAKE_EXPANDED = false;
        
    } else if (INTAKE_IS_REVERSING && !hw->controller.ButtonL2.pressing()) {
        stop_intake();
        left_right_joystick_multiplier = HIGH_DRIVETRAIN_VELOCITY;
        INTAKE_IS_REVERSING = false;
    } else if (hw->controller.ButtonR2.pressing()) {
        if (!INTAKE_EXPANDED || !hw->intake.isSpinning()) {
            expand_intake();
            CATAPULT_DISABLED = false;
            left_right_joystick_multiplier = LOW_DRIVETRAIN_VELOCITY;
            hw->right_intake_motor.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
            INTAKE_HELD = true;
        }
        INTAKE_EXPANDED = true;
        intake_count = 0;
    } else if (hw->controller.ButtonL2.pressing()) {
        if (INTAKE_EXPANDED) {
            retract_intake();
            CATAPULT_DISABLED = true;
            left_right_joystick_multiplier = HIGH_DRIVETRAIN_VELOCITY;
            // stop_intake();
            INTAKE_HELD = false;
            intake_count = 0;
        }
        reverse_intake();
        INTAKE_IS_REVERSING = true;
    }
    if (intake_count >= rc->INTAKE_WAIT) stop_intake_expansion();
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

    if (hw->controller.ButtonB.pressing()) {
        if (!CATAPULT_STRATEGY_RAN) {
            // Expand intake
            INTAKE_EXPANDED = true;
            expand_intake();
            vex::wait(500, vex::timeUnits::msec);
            stop_intake_expansion();
        }
        if (rc->ROBOT == SCRATETTE) {
            expand_intake();
            vex::wait(100, vex::timeUnits::msec);
            stop_intake_expansion();
        }
            run_catapult_once(); 
        CATAPULT_STRATEGY_RAN = true;
    } else {
        CATAPULT_STRATEGY_RAN = false;
    }

}

void UserDrive::climb_controls()
{
    if (hw->controller.ButtonDown.pressing()) {
        hw->right_climb_motor.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
        hw->left_climb_motor.spin(vex::directionType::fwd, 12.0, vex::voltageUnits::volt);
    } else if(hw->controller.ButtonUp.pressing()) {
        hw->right_climb_motor.spin(vex::directionType::fwd, 12.0, vex::voltageUnits::volt);
        hw->left_climb_motor.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
    } else {
        hw->right_climb_motor.stop();
        hw->left_climb_motor.stop();

    }

    if (hw->controller.ButtonLeft.pressing()) {
        hw->climb_lock.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
    } else if (hw->controller.ButtonRight.pressing()) {
        hw->climb_lock.spin(vex::directionType::fwd, 12.0, vex::voltageUnits::volt);
    } else {
        hw->climb_lock.stop();
    }
}

void UserDrive::last_chance() {
    if (hw->controller.ButtonY.pressing()) {
        catapult_task.stop();
        catapult_task = vex::task(run_catapult_thread, this, 2);
        hw->catapult.spin(vex::directionType::rev, 6.0, vex::voltageUnits::volt);
    }
}