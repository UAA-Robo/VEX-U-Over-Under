#include "AutoDrive.h"


AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) {}

void AutoDrive::drive() {
    rotate_and_drive_to_position({24, 0});
    rotate_and_drive_to_position({24, 60});
    rotate_and_drive_to_position({0, 0});

    rotate_to_relative_angle(115);
}

void AutoDrive::rotate_to_relative_angle(double angle) // Based on ENCODERS,
{

    double number_drivetrain_revolutions = angle * (rc->DRIVETRAINWIDTH) * M_PI / (360 * rc->WHEELCIRC);
    double revolutions_left_wheels = -number_drivetrain_revolutions;
    double revolutions_right_wheels = number_drivetrain_revolutions;

    std::pair<double, double> vel = calculate_drivetrain_velocity({0, rc->auto_rotate_velocity_percent});

    hw->left_drivetrain_motors.spinFor(revolutions_left_wheels, vex::rotationUnits::rev, vel.first, vex::velocityUnits::pct, false);
    hw->right_drivetrain_motors.spinFor(revolutions_right_wheels, vex::rotationUnits::rev, vel.second, vex::velocityUnits::pct);
    while (fabs(hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0 || 
        fabs(hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0); // Blocks other tasks from starting
}

void AutoDrive::rotate_to_heading_odometry(double heading)
{
    // Corrects heading to be from 0-360 from the x axis counterclockwise if applicable
    heading = fmod(heading, 360);
    if (heading < 0)
        heading += 360;

    // Determines whether to rotate left or right based on the  shortest distance
    // Check if spin- is non-blocking
    // Clockwise
    if (fabs(heading - tm->get_current_heading()) > 180) {
        hw->left_drivetrain_motors.spin(vex::directionType::fwd);
        hw->right_drivetrain_motors.spin(vex::directionType::rev);
    }
    // Counter-clockwise
    else {
        hw->left_drivetrain_motors.spin(vex::directionType::rev);
        hw->right_drivetrain_motors.spin(vex::directionType::fwd);
    }
    // range -1 to 1...for now
    while (fabs(heading - tm->get_current_heading()) > 1);   // Stop motors

    hw->left_drivetrain_motors.stop();                       // TODO: Give a vel value later
    hw->right_drivetrain_motors.stop();


}


void AutoDrive::rotate_to_position(std::pair<double, double> final_position, bool ISBACKROTATION)
{
    // if (IS_USING_GPS_POSITION) tm->set_current_heading(tm->getGPSPosition());
    double heading = tm->get_heading_between_points(tm->get_current_position(), final_position);

    if (ISBACKROTATION)
        heading -= 180;
    rotate_to_heading_odometry(heading);
}

void AutoDrive::rotate_and_drive_to_position(std::pair<double, double> position, bool ISBACKTOPOSITION)
{

    rotate_to_position(position, ISBACKTOPOSITION);

    // if (IS_USING_GPS_HEADING) tm->set_current_heading(tm->getGPSPosition());

    double distance_to_position = tm->get_distance_between_points(tm->get_current_position(), position); // inches
    move_drivetrain_distance_odometry(distance_to_position, ISBACKTOPOSITION, position);    // TODO: Calculate initial position within function
}

