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

void Drive::move_to_position(bool ISBACKTOPOSITION, std::pair<double, double> position)
{   
    double distance_to_position = tm->get_distance_between_points(tm->get_current_position(), position); // TODO: remove when moved to drive

    // Check if reached
    // range -0.5 to 0.5 ...for now
    while (fabs(distance_to_position) > 0.5) {  

        std::cout << "Distance: " << distance_to_position << std::endl; 
        std::cout << "CurrentX: " << tm->get_current_position().first << std::endl;
        std::cout << "CurrentY: " << tm->get_current_position().second << std::endl;

        if (ISBACKTOPOSITION)   // Backward
        {
            hw->left_drivetrain_motors.spinTo(-360, vex::rotationUnits::deg, 10.0, vex::velocityUnits::pct, false);
            hw->right_drivetrain_motors.spinTo(-360, vex::rotationUnits::deg, 10.0, vex::velocityUnits::pct, false);
        }
        else                    // Forward
        {
            hw->left_drivetrain_motors.spinTo(360, vex::rotationUnits::deg, 10.0, vex::velocityUnits::pct, false);
            hw->right_drivetrain_motors.spinTo(360, vex::rotationUnits::deg, 10.0, vex::velocityUnits::pct, false);
        }          

        distance_to_position = tm->get_distance_between_points(tm->get_current_position(), position);

        vex::wait(20, vex::timeUnits::msec);    // wait for telem to update
    }

    // stop wheels
    hw->left_drivetrain_motors.stop();
    hw->right_drivetrain_motors.stop();
}
