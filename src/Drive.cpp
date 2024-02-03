#include "Drive.h"

Drive::Drive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry)
{
    hw = hardware;
    rc = robotConfig;
    tm = telemetry;

    //setup_bot();
    // pid = new PID(rc, tm);
}

std::pair<double, double> Drive::calculate_drivetrain_velocity(std::pair<double, double> velocity_percent)
{
    double vertical_velocity_percent = velocity_percent.first / 100;
    double horizontal_velocity_percent = velocity_percent.second / 110; // To reduce sensitivity when turning 

    // Calculate raw left and right motor velocity
    double raw_left_velocity = vertical_velocity_percent + horizontal_velocity_percent;
    double raw_right_velocity = vertical_velocity_percent - horizontal_velocity_percent;

    // Normalize the motor velocity
    double max_raw_velocity = std::max(std::abs(raw_left_velocity), std::abs(raw_right_velocity));
    double normalization_factor = max_raw_velocity > 1.0 ? max_raw_velocity : 1.0;

    double left_velocity_multiplier = raw_left_velocity / normalization_factor;
    double right_velocity_multiplier = raw_right_velocity / normalization_factor;

    // Multiplying by 100 breaks it?? Why?
    double left_velocity = 95 * left_velocity_multiplier;
    double right_velocity = 95 * right_velocity_multiplier;

    return {left_velocity, right_velocity};
}

void Drive::move_drivetrain(std::pair<double, double> velocity_percent)
{
    std::pair<double, double> velocity = calculate_drivetrain_velocity(velocity_percent);

    hw->left_drivetrain_motors.spin(vex::directionType::fwd, velocity.first, 
                                    vex::velocityUnits::pct);
    hw->right_drivetrain_motors.spin(vex::directionType::fwd, velocity.second, 
                                    vex::velocityUnits::pct);
    
    // if (rc->USE_PID_IN_USERDRIVE) pid->correct_drive();
}

void Drive::activate_intake()
{
    if (rc->ROBOT == SCRAT) hw->intake.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
    else hw->intake.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
    hw->controller.Screen.setCursor(1, 1);
    hw->controller.Screen.print("Activating Intake!");
}


void Drive::stop_intake()
{
    hw->intake.stop();
}


void Drive::expand_intake()
{
    hw->intake_expansion.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);
    hw->controller.Screen.setCursor(1, 1);
    hw->controller.Screen.print("Expanding Intake!");
}


void Drive::retract_intake()
{
    hw->intake_expansion.spin(vex::directionType::rev, 6, vex::voltageUnits::volt);
    hw->controller.Screen.setCursor(1, 1);
    hw->controller.Screen.print("Retracting Intake!");
}


void Drive::stop_intake_expansion()
{
    hw->intake_expansion.stop(vex::brakeType::coast);
}

void Drive::snowplow_out() {
    hw->right_plow.set(false);
    hw->left_plow.set(false);
    SNOWPLOW_OUT = true;
}

void Drive::right_snowplow_out() {
    hw->right_plow.set(false);
    SNOWPLOW_OUT = true;
}

void Drive::left_snowplow_out() {
    hw->left_plow.set(false);
    SNOWPLOW_OUT = true;
}

void Drive::snowplow_in() {
    hw->right_plow.set(true);
    hw->left_plow.set(true);
    SNOWPLOW_OUT = false;
}

void Drive::right_snowplow_in() {
    hw->right_plow.set(true);
    if (!hw->right_plow.value() || !hw->left_plow.value()) {
        SNOWPLOW_OUT = true;
    }
    else {
        SNOWPLOW_OUT = false;
    }
}

void Drive::left_snowplow_in() {
    hw->left_plow.set(true);
    if (!hw->right_plow.value() || !hw->left_plow.value()) {
        SNOWPLOW_OUT = true;
    }
    else {
        SNOWPLOW_OUT = false;
    }
}

int Drive::run_catapult_thread(void* param)
{
    // WARNING: DON'T print in this thread or it will take too long and miss the catapult press
    Drive* dr = static_cast<Drive*>(param);
    dr->hw->catapult_sensor.resetPosition();
    

    // 0 deg is really 358-2 deg
    double MAX_ANGLE = dr->rc->MAX_CATAPULT_ANGLE - 5; // To slow down early
    double angle; 
    while(true) {
        // STOP catapult
        angle = dr->hw->catapult_sensor.angle(vex::deg);
        if (angle >= MAX_ANGLE && angle <= 350 && !dr->START_CATAPULT_LAUNCH) {
            // Robots need downward force to stop catapult
            dr->hw->catapult.stop();
            if (dr->rc->ROBOT== SCRAT) dr->hw->catapult.spin(vex::directionType::rev, 1.0, vex::voltageUnits::volt);
            else  dr->hw->catapult.spin(vex::directionType::rev, 1, vex::voltageUnits::volt);
            
        } else  {
            dr->hw->catapult.spin(vex::directionType::rev, 6.0, vex::voltageUnits::volt);
        }

        // if (!dr->START_CATAPULT_LAUNCH) {
        //     dr->hw->catapult.stop();
        // } else  {
        //     dr->hw->catapult.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
        // }
        vex::wait(10, vex::timeUnits::msec);
    }
}

void Drive::start_catapult() {
    START_CATAPULT_LAUNCH = true;
}

void Drive::stop_catapult() {
    START_CATAPULT_LAUNCH = false;
}

// void Drive::setup_bot() {

//     // Expand intake
//     std::cout << "EXPANDING" << std::endl;
//     expand_intake();
//     vex::wait(500, vex::timeUnits::msec);
//     stop_intake_expansion();

//     std::cout << "CATAPULTING" << std::endl;
//     // Start catapult thread and close catapult
//     vex::task catapult_task = vex::task(run_catapult_thread, this, 2);

//     start_catapult();
//     vex::wait(500, vex::timeUnits::msec);
//     stop_catapult();

//     vex::wait(1000, vex::timeUnits::msec);

//     std::cout << "RETRACTING" << std::endl;
//     // Retract intake
//     retract_intake();
//     vex::wait(500, vex::timeUnits::msec);
//     stop_intake_expansion();
//     //vex::wait(5000, vex::timeUnits::msec);

//     std::cout << "ENDING" << std::endl;

// }

void Drive::run_catapult_catapult_strategy(int number_triballs) {

    
    double velocity = 50;
    if (rc->ROBOT == SCRATETTE) velocity = 50;

    // Expand intake
    expand_intake();
    if (rc->ROBOT == SCRAT)  vex::wait(500, vex::timeUnits::msec);
    vex::wait(500, vex::timeUnits::msec);
    stop_intake_expansion();

    // Start Catapult thread
    vex::task catapult_task = vex::task(run_catapult_thread, this, 1);

    // Launch triball
    start_catapult();
    vex::wait(500, vex::timeUnits::msec);
    stop_catapult();

    // Start intake
    activate_intake();

    // Move + Launch
    for (int i = 0; i < number_triballs - 1; i++) {
        turbo_drive_distance(6, true, velocity);

        start_catapult();
        vex::wait(500, vex::timeUnits::msec);
        stop_catapult();
        if (rc->ROBOT == SCRATETTE) turbo_drive_distance(7, false, velocity);
        else turbo_drive_distance(6.2, false, velocity);
    }

    // Launches last triball and finishes outword
    turbo_drive_distance(6, true, velocity);
    start_catapult();
    vex::wait(500, vex::timeUnits::msec);
    stop_catapult();

    stop_intake();
    retract_intake();
    vex::wait(500, vex::timeUnits::msec);
    if (rc->ROBOT == SCRAT)  vex::wait(500, vex::timeUnits::msec);
    stop_intake_expansion();

}


void Drive::turbo_drive_distance(double distance, bool IS_REVERSE, double velocity) {

    double num_wheel_revolutions = distance / rc->WHEEL_CIRCUMFERENCE;
    
    // std::pair<double, double> vel = calculateDriveTrainVel(velPercent);

    hw->drivetrain.resetPosition();

    if (IS_REVERSE) num_wheel_revolutions = -num_wheel_revolutions;
    hw->drivetrain.spinTo(num_wheel_revolutions, vex::rotationUnits::rev, velocity, vex::velocityUnits::pct, false);
    
    vex::wait(50, vex::timeUnits::msec);
    while(fabs(hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0.0 || fabs(hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0.0); //Blocks other tasks from starting 


}