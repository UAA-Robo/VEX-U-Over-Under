#include "Drive.h"
#include <iostream>

Drive::Drive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry)
{
    hw = hardware;
    rc = robotConfig;
    tm = telemetry;
}

std::pair<double, double> Drive::calculate_drivetrain_velocity(std::pair<double, double> velocity_percent) //{vertical_velocity_percent, horizontal_velocity_percent}
{
    double vertical_velocity_percent = velocity_percent.first / 100;
    double horizontal_velocity_percent = velocity_percent.second / 100;

    // Calculate raw left and right motor velocity
    double raw_left_velocity = vertical_velocity_percent + horizontal_velocity_percent;  // raw velocity of left wheels in drive train
    double raw_right_velocity = vertical_velocity_percent - horizontal_velocity_percent; // raw velocity of right wheels in drive train

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

void Drive::move_drivetrain_distance(std::pair<double, double> velocity_percent, double distance)
{
    double number_wheel_revolutions = distance / rc->WHEELCIRC;

    std::pair<double, double> velocity = calculate_drivetrain_velocity(velocity_percent);

    // Left
    hw->front_left_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.first, vex::velocityUnits::pct, false);
    hw->middle_left_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.first, vex::velocityUnits::pct, false);
    hw->back_left_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.second, vex::velocityUnits::pct, false);

    // Right
    // Unsure of true/false values
    hw->back_right_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.second, vex::velocityUnits::pct, false);
    hw->middle_right_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.first, vex::velocityUnits::pct, false);
    hw->front_right_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev, velocity.second, vex::velocityUnits::pct);
