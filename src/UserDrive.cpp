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
        catapult_task = vex::task(run_catapult_thread_plz_work, this, 2);

    while(true) {

        activate_catapult_strategy();
        drivetrain_controls();
        catapult_controls();
        intake_controls();
        snowplow_controls();
        climb_controls();
        last_chance();

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
    }
}


void UserDrive::catapult_controls()
{
    if (hw->controller.ButtonL1.pressing() && !CATAPULT_DISABLED) start_catapult();
    else stop_catapult();
}


void UserDrive::intake_controls()
{
    if (hw->controller.ButtonX.pressing()) {
        stop_intake();
        left_right_joystick_multiplier = 0.8;
    } else if (hw->controller.ButtonR1.pressing()) {
        if (!INTAKE_EXPANDED || INTAKE_HELD) {
            INTAKE_HELD = false;
            expand_intake();
            activate_intake();
            CATAPULT_DISABLED = false;
            left_right_joystick_multiplier = 0.4;    
        }
        INTAKE_EXPANDED = true;
        intake_count = 0;
    } else if (!hw->controller.ButtonR2.pressing() && !hw->controller.ButtonL2.pressing()
    && !INTAKE_HELD && !INTAKE_IS_REVERSING) {
        if (INTAKE_EXPANDED) {
            retract_intake();
            stop_intake();
            CATAPULT_DISABLED = true;
            left_right_joystick_multiplier = 0.8;
            INTAKE_HELD = false;
        }
        INTAKE_EXPANDED = false;
        intake_count = 0;
    } else if (INTAKE_IS_REVERSING && !hw->controller.ButtonL2.pressing()) {
        stop_intake();
        left_right_joystick_multiplier = 0.8;
        INTAKE_IS_REVERSING = false;
    } else if (hw->controller.ButtonR2.pressing()) {
        if (!INTAKE_EXPANDED || !hw->intake.isSpinning()) {
            expand_intake();
            CATAPULT_DISABLED = false;
            left_right_joystick_multiplier = 0.4;
            hw->right_intake_motor.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
            INTAKE_HELD = true;
        }
        INTAKE_EXPANDED = true;
        intake_count = 0;
    } else if (hw->controller.ButtonL2.pressing()) {
        if (INTAKE_EXPANDED) {
            retract_intake();
            CATAPULT_DISABLED = true;
            left_right_joystick_multiplier = 0.8;
            // stop_intake();
            INTAKE_HELD = false;
        }
        reverse_intake();
        INTAKE_IS_REVERSING = true;
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

int UserDrive::run_catapult_thread_plz_work(void* param)
{
    std::cout << "Plz work version Created\n";
    // WARNING: DON'T print in this thread or it will take too long and miss the catapult press
    UserDrive* dr = static_cast<UserDrive*>(param);
    dr->hw->catapult_sensor.resetPosition();
    

    // 0 deg is really 358-2 deg
    double MAX_ANGLE = dr->rc->MAX_CATAPULT_ANGLE - 5; // To slow down early
    double angle; 
    while(true) {
        // STOP catapult
        angle = dr->hw->catapult_sensor.angle(vex::deg);
        if (dr->START_CATAPULT_LAUNCH) {
            std::cout << "Starting catapult launch\n";
        }
        if (angle >= MAX_ANGLE && angle <= 350 && !dr->START_CATAPULT_LAUNCH) {
            // Robots need downward force to stop catapult
            dr->hw->catapult.stop();
            std::cout << "1 volt\n";
            dr->hw->catapult.spin(vex::directionType::rev, 1, vex::voltageUnits::volt);
            
        } else  {
            std::cout << "12 volts\n";
            dr->hw->catapult.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
            dr->hw->left_drivetrain_motors.spin(vex::directionType::fwd, 4.0, vex::voltageUnits::volt);
        }
        
        vex::wait(10, vex::timeUnits::msec);
    }
}

void UserDrive::last_chance() {
    if (hw->controller.ButtonY.pressing()) {
        catapult_task.stop();
        catapult_task = vex::task(run_catapult_thread_plz_work, this, 2);
    }
}