#include "Drive.h"
#include <iostream>

Drive::Drive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry)
{
    hw = hardware;
    rc = robotConfig;
    tm = telemetry;
}

std::pair<double, double> Drive::calculate_drivetrain_velocity(
    std::pair<double, double> velocity_percent)
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
}

void Drive::move_drivetrain_distance(std::pair<double, double> velocity_percent, double distance)
{
    // (4 * 3.14) should be rc->WHEELCIRC in the future
    double number_wheel_revolutions = distance / (4 * 3.14);

    std::pair<double, double> velocity = calculate_drivetrain_velocity(velocity_percent);

    // Left
    hw->front_left_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev,
                                            velocity.first, vex::velocityUnits::pct, false);
    hw->middle_left_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev,
                                             velocity.first, vex::velocityUnits::pct, false);
    hw->back_left_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev,
                                           velocity.second, vex::velocityUnits::pct, false);

    // Right
    hw->back_right_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev,
                                            velocity.second, vex::velocityUnits::pct, false);
    hw->middle_right_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev,
                                              velocity.first, vex::velocityUnits::pct, false);
    hw->front_right_drivetrain_motor.spinFor(number_wheel_revolutions, vex::rotationUnits::rev,
                                             velocity.second, vex::velocityUnits::pct);
}

std::pair<double, double> Drive::calculateDriveTrainVel(std::pair<double, double> velPercent) //{verticalVelPercent, horizontalVelPercent}
{
    double verticalVelPercent = velPercent.first / 100;
    double horizontalVelPercent = velPercent.second / 100;

    // Calculate raw left and right motor velocity
    double rawLeftVel = verticalVelPercent + horizontalVelPercent;  // raw velocity of left wheels in drive train
    double rawRightVel = verticalVelPercent - horizontalVelPercent; // raw velocity of right wheels in drive train

    // Normalize the motor velocity
    double maxRawVel = std::max(std::abs(rawLeftVel), std::abs(rawRightVel));
    double normalizationFactor = maxRawVel > 1.0 ? maxRawVel : 1.0;

    double leftVelMultiplier = rawLeftVel / normalizationFactor;
    double rightVelMultiplier = rawRightVel / normalizationFactor;

    double leftVel = 100 * leftVelMultiplier;
    double rightVel = 100 * rightVelMultiplier;

    return {leftVel, rightVel};
}

void Drive::moveDriveTrain(std::pair<double, double> velPercent)
{
    std::pair<double, double> vel = calculateDriveTrainVel(velPercent);

    hw->left_drivetrain_motors.spin(vex::directionType::fwd, vel.first, vex::velocityUnits::pct);
    hw->right_drivetrain_motors.spin(vex::directionType::fwd, vel.second, vex::velocityUnits::pct);
}

void Drive ::moveDriveTrainDistance(std::pair<double, double> velPercent, double distance)
{
    double numberWheelRevolutions = distance / rc->WHEELCIRC;

    std::pair<double, double> vel = calculateDriveTrainVel(velPercent);

    hw->wheelLeftBack.spinFor(numberWheelRevolutions, vex::rotationUnits::rev, vel.first, vex::velocityUnits::pct, false);
    hw->wheelLeftFront.spinFor(numberWheelRevolutions, vex::rotationUnits::rev, vel.first, vex::velocityUnits::pct, false);
    hw->wheelRightBack.spinFor(numberWheelRevolutions, vex::rotationUnits::rev, vel.second, vex::velocityUnits::pct, false);
    hw->wheelRightFront.spinFor(numberWheelRevolutions, vex::rotationUnits::rev, vel.second, vex::velocityUnits::pct);

    vex::wait(50, vex::timeUnits::msec);
    while (fabs(hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0 || fabs(hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0)
        ; // Blocks other tasks from starting

    double currHeading = tm->getCurrHeading();
    double newX = tm->getCurrPosition().first + distance * cos(currHeading * M_PI / 180.0); // need to convert degrees to radians
    double newY = tm->getCurrPosition().second + distance * sin(currHeading * M_PI / 180.0);
    tm->setCurrPosition({newX, newY});
    tm->positionErrorCorrection();
    tm->headingErrorCorrection();
}
