#include "Drive.h"
#include <iostream>

Drive::Drive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry)
{
    hw = hardware;
    rc = robotConfig;
    tm = telemetry;
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

    hw->left_drivetrain_motors.spin(vex::directionType::fwd, velocity.first, vex::velocityUnits::pct);
    hw->right_drivetrain_motors.spin(vex::directionType::fwd, velocity.second, vex::velocityUnits::pct);
}

// void Drive::move_drivetrain_distance(std::pair<double, double> velocity_percent, double distance)
// {
//     // (4 * 3.14) should be rc->WHEELCIRC in the future
//     double number_wheel_revolutions = distance / rc->WHEELCIRC;

//     std::pair<double, double> velocity = calculate_drivetrain_velocity(velocity_percent);

//     // Left
//     hw->front_left_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.first, vex::velocityUnits::pct, false);
//     hw->middle_left_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.first, vex::velocityUnits::pct, false);
//     hw->back_left_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.second, vex::velocityUnits::pct, false);

//     // Right
//     hw->back_right_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.second, vex::velocityUnits::pct, false);
//     hw->middle_right_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.first, vex::velocityUnits::pct, false);
//     hw->front_right_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.second, vex::velocityUnits::pct);

//     vex::wait(50, vex::timeUnits::msec);
//     while (fabs(hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0 || 
//         fabs(hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0); // Blocks other tasks from starting

//     double curr_heading = tm->get_current_heading();
//     double newX = tm->get_current_position().first + distance * cos(curr_heading * M_PI / 180.0); // need to convert degrees to radians
//     double newY = tm->get_current_position().second + distance * sin(curr_heading * M_PI / 180.0);
//     tm->set_current_heading({newX, newY});
// }



// void Drive::move_drivetrain_distance_odometry(double distance, bool ISBACKTOPOSITION, std::pair<double, double> position)
// {
//     //  TODO: Calculate initial distance in function
//     //  Remove double distance parameter

//     if (ISBACKTOPOSITION)   // Backward
//     {
//         hw->left_drivetrain_motors.spin(vex::directionType::rev);
//         hw->right_drivetrain_motors.spin(vex::directionType::rev);
//     }
//     else                    // Forward
//     {
//         hw->left_drivetrain_motors.spin(vex::directionType::fwd);
//         hw->right_drivetrain_motors.spin(vex::directionType::fwd);  
//     }

//     // Check if reached
//     // range -1 to 1...for now
//     while (fabs(distance) > 1) {             // stop wheels
//         hw->left_drivetrain_motors.stop();
//         hw->right_drivetrain_motors.stop();

//         // update distance from final position (goal)
//         distance = tm->get_distance_between_points(tm->get_current_position(), position);
//     }

// }

void Drive::move_drivetrain_distance_odometry(double distance, bool ISBACKTOPOSITION, std::pair<double, double> position)
{

    // Check if reached
    // range -1 to 1...for now
    while (fabs(distance) > 1) {   

        if (ISBACKTOPOSITION)   // Backward
        {
            hw->left_drivetrain_motors.spinTo(-90, vex::rotationUnits::deg, 100.0, vex::velocityUnits::pct, false);
            hw->right_drivetrain_motors.spinTo(-90, vex::rotationUnits::deg, 100.0, vex::velocityUnits::pct, false);
        }
        else                    // Forward
        {
            hw->left_drivetrain_motors.spinTo(90, vex::rotationUnits::deg, 100.0, vex::velocityUnits::pct, false);
            hw->right_drivetrain_motors.spinTo(90, vex::rotationUnits::deg, 100.0, vex::velocityUnits::pct, false);
        }          

        // update distance from final position (goal)
        distance = tm->get_distance_between_points(tm->get_current_position(), position);
    }

    // stop wheels
    hw->left_drivetrain_motors.stop();
    hw->right_drivetrain_motors.stop();

}
