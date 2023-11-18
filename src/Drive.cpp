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
//     while (fabs(distance) > 1) {

//         // update distance from final position (goal)
//         distance = tm->get_distance_between_points(tm->get_current_position(), position);
//         std::cout << "Distance: " << distance << std::endl;
//     }
//     hw->left_drivetrain_motors.stop();      // stop wheels
//     hw->right_drivetrain_motors.stop();

// }


// new function
void Drive::move_drivetrain_distance_odometry(double distance, bool ISBACKTOPOSITION, std::pair<double, double> position) 
{

    // Check if reached
    // range -0.05 to 0.05...for now // check with 5 inches
    while (fabs(distance) > 0.05) {

        if(ISBACKTOPOSITION)  // Move Back
        {
        hw->left_drivetrain_motors.spinTo(-90, vex::rotationUnits::deg, 100.0, vex::velocityUnits::pct, false);
        hw->right_drivetrain_motors.spinTo(-90, vex::rotationUnits::deg, 100.0, vex::velocityUnits::pct, false);
        }
        else                  // Move forward
        {
        hw->left_drivetrain_motors.spinTo(90, vex::rotationUnits::deg, 100.0, vex::velocityUnits::pct, false);
        hw->right_drivetrain_motors.spinTo(90, vex::rotationUnits::deg, 100.0, vex::velocityUnits::pct, false);
        }

        // update distance from final position (goal)
        distance = tm->get_distance_between_points(tm->get_current_position(), position);
    }

    hw->left_drivetrain_motors.stop();      // stop wheels
    hw->right_drivetrain_motors.stop();
}
