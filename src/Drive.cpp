#include "Drive.h"

Drive::Drive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry)
{
    hw = hardware;
    rc = robotConfig;
    tm = telemetry;
    // pid = new PID(rc, tm);
}

std::pair<double, double> Drive::calculate_drivetrain_velocity(std::pair<double, double> velocity_percent)
{
    double vertical_velocity_percent = velocity_percent.first / 100;
    double horizontal_velocity_percent = velocity_percent.second / 100;

    // Calculate raw left and right motor velocity
    double raw_left_velocity = vertical_velocity_percent + horizontal_velocity_percent;
    double raw_right_velocity = vertical_velocity_percent - horizontal_velocity_percent;

    // Normalize the motor velocity
    double max_raw_velocity = std::max(std::abs(raw_left_velocity), std::abs(raw_right_velocity));
    double normalization_factor = max_raw_velocity > 1.0 ? max_raw_velocity : 1.0;

    double left_velocity_multiplier = raw_left_velocity / normalization_factor;
    double right_velocity_multiplier = raw_right_velocity / normalization_factor;

    double left_velocity = 100 * left_velocity_multiplier;
    double right_velocity = 100 * right_velocity_multiplier;

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
    if (rc->ROBOT == SCRAT) hw->intake.spin(vex::directionType::rev, 8.0, vex::voltageUnits::volt);
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
    hw->intake_expansion.stop();
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
    bool CATAPULT_STOPPED = false;
    while(true) {
        //STOP catapult
        // Prevents catapult/limit switch from bouncing
        if (dr->hw->catapult_limit_switch.value() == 1 && !dr->START_CATAPULT_LAUNCH) {
            CATAPULT_STOPPED = true;
        }

        if (dr->START_CATAPULT_LAUNCH) CATAPULT_STOPPED = false;

        if (CATAPULT_STOPPED) {
            if (dr->rc->ROBOT == SCRAT) {
                dr->hw->catapult.stop();
            } else {  // Scrattete needs force down to stop catapult since no rachette
                dr->hw->catapult.spin(vex::directionType::rev, 1, vex::voltageUnits::volt);
            };
            
        } else  {
            dr->hw->catapult.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
            CATAPULT_STOPPED = false;
        }
        vex::wait(10, vex::timeUnits::msec);
    }
}

void Drive::launch_catapult() {
    START_CATAPULT_LAUNCH = true;
}

void Drive::stop_catapult() {
    START_CATAPULT_LAUNCH = false;
}