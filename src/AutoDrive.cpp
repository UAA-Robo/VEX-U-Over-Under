#include "AutoDrive.h"


AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) {}

void AutoDrive::drive() {
    // rotate_and_drive_to_position({24, 0});
    // rotate_and_drive_to_position({24, 60});
    // rotate_and_drive_to_position({0, 0});
    rotate_and_drive_to_position({10,0});

    //rotate_to_relative_angle(115);
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
    // range -0.05 to 0.05...for now
    while (fabs(heading - tm->get_current_heading()) > 0.05);       // Stop motors

    hw->left_drivetrain_motors.stop();                          // TODO: Give a vel value later
    hw->right_drivetrain_motors.stop();




}


void AutoDrive::rotate_to_position(std::pair<double, double> final_position, bool ISBACKROTATION)
{
    // if (IS_USING_GPS_POSITION) tm->set_current_heading(tm->getGPSPosition());
    double heading = tm->get_heading_between_points(tm->get_current_position(), final_position);

    if (ISBACKROTATION)
        heading -= 180;
    rotate_to_heading_odometry(heading); //changed to odometry function

}

void AutoDrive::rotate_and_drive_to_position(std::pair<double, double> position, bool ISBACKTOPOSITION)
{

    rotate_to_position(position, ISBACKTOPOSITION);

    // if (IS_USING_GPS_HEADING) tm->set_current_heading(tm->getGPSPosition());

    double distance_to_position = tm->get_distance_between_points(tm->get_current_position(), position); // inches
    if (ISBACKTOPOSITION)
        distance_to_position = -distance_to_position;
        
    // move_drivetrain_distance({rc->auto_drive_velocity_percent, 0}, distance_to_position); // Drive at auto_drive_velocity_percent% velocity
    //changed to odometry function
    move_drivetrain_distance_odometry(distance_to_position, ISBACKTOPOSITION, position);
}

