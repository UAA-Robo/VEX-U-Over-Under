#include "Drive.h"

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
void Drive::move_drivetrain_distance_odometry(std::pair<double, double> position, bool ISBACKTOPOSITION) 
{

    // update distance from final position (goal)
    double distance = tm->get_distance_between_points(tm->get_current_position(), position);
    double distance_goal = distance;



    // Check if reache
    // range -0.05 to 0.05...for now // check with 5 inches
    // m+atan(x)−atan(x-D)−1.5
    // m is minimum speed, D is distance, x is distance travelled
    
    double min_velocity = 1;
    double max_velocity = 30;
    double velocity;
    if (ISBACKTOPOSITION) {
        hw->drivetrain.spin(vex::directionType::rev, min_velocity, vex::velocityUnits::pct);
    } else {
        hw->drivetrain.spin(vex::directionType::fwd, min_velocity, vex::velocityUnits::pct);
    }


    while (fabs(distance) > 0.5) {
        distance = tm->get_distance_between_points(tm->get_current_position(), position);
        // Slows down when leaving or approaching destination
        velocity = (atan(distance_goal - distance) + atan(distance)) / M_PI 
            * (max_velocity - min_velocity) + min_velocity; 
        hw->drivetrain.setVelocity(velocity, vex::velocityUnits::pct);
        //std::cout << "Drive: " << velocity << "," << tm->odometry_x_position << "," << tm->odometry_y_position << "," << tm->odometry_heading << " |"<< distance << std::endl;
        vex::wait(10, vex::timeUnits::msec);
    }

    hw->left_drivetrain_motors.stop();      // stop wheels
    hw->right_drivetrain_motors.stop();
}
