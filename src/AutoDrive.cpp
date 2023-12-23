#include "AutoDrive.h"


AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) {}

void AutoDrive::drive() {
    // Set braking
    hw->left_drivetrain_motors.setStopping(vex::brakeType::brake);
    hw ->right_drivetrain_motors.setStopping(vex::brakeType::brake);

    // rotate_to_heading_odometry(340);
    // vex::wait(2, vex::timeUnits::sec);
    // rotate_to_heading_odometry(90);

    // while(true) {
    //     vex::wait(25, vex::timeUnits::msec);
    // };
    rotate_and_drive_to_position({24, 0});
    vex::wait(2, vex::timeUnits::sec);
    rotate_and_drive_to_position({12, -12});
    vex::wait(2, vex::timeUnits::sec);
    rotate_and_drive_to_position({0, 0}, true);


    //rotate_and_drive_to_position({0, 0});
    //rotate_and_drive_to_position({24, 60});
    // rotate_and_drive_to_position({0, 0});
    // Assume heading, x and y are all 0
    //rotate_to_heading_odometry(360);

    //move_drivetrain_distance_odometry({24,0});
    //move_drivetrain_distance_odometry({48,0});

    //rotate_to_relative_angle(115);
}

void AutoDrive::rotate_to_heading_odometry(double heading)
{
    // Corrects heading to be from 0-360 from the x axis counterclockwise if applicable
    heading = fmod(heading, 360);
    if (heading < 0) heading += 360;
    double min_velocity = 10;
    double max_velocity = 50;
    double stopping_aggression = 0.01; // higher number is higher aggression (steeper slope)
    double velocity;


    
    // 1 if counterclockwise, -1 if clockwise depending on which direction is shorter to turn
    double turn_direction = ((fabs(heading - tm->get_current_heading()) > 180) ^ (heading > tm->get_current_heading())) ? 1: -1;


    // Total angle to travel is shorter distance
    double angle_to_travel = fabs(heading - tm->get_current_heading());
    if (angle_to_travel > 180) angle_to_travel = 360 - angle_to_travel;

    // Slow for small angles bc function doesn't slow things down enough. TODO modify function
    if (angle_to_travel < 30) {
        min_velocity = 10;
        max_velocity = 14;
    }

    double total_angle_to_travel = angle_to_travel;

    // Turn wheels opposite of each other
    hw->left_drivetrain_motors.spin(vex::directionType::fwd, -min_velocity * turn_direction, vex::velocityUnits::pct);
    hw->right_drivetrain_motors.spin(vex::directionType::fwd, min_velocity * turn_direction, vex::velocityUnits::pct);
    double previous_angle_to_travel = angle_to_travel + 1;
    // Turn until within 2 degrees of desired heading
    while (angle_to_travel > 1 && (previous_angle_to_travel - angle_to_travel) >= -0.05) {
        // Speeds up as leaving initial angle and slows down as approaching destination
        if (angle_to_travel > total_angle_to_travel/2) {
            // First half of distance
            velocity = atan(fabs(total_angle_to_travel - angle_to_travel)) * 2 * (max_velocity-min_velocity) / M_PI + min_velocity;
            //std::cout << "Here1: " << velocity << std::endl;
        } else {
            // Second half of distance
            velocity = atan(stopping_aggression * angle_to_travel) * 2 * max_velocity / M_PI;
            //std::cout << "Here2: " << velocity << std::endl;
        }

        hw->left_drivetrain_motors.setVelocity(-velocity * turn_direction, vex::velocityUnits::pct);
        hw->right_drivetrain_motors.setVelocity(velocity * turn_direction, vex::velocityUnits::pct);
        
        vex::wait(30, vex::timeUnits::msec);
        std::cout << "Rotate: " << heading << ", " << angle_to_travel << ", " << velocity << "," << tm->odometry_x_position << "," << tm->odometry_y_position << "," << tm->odometry_heading << std::endl;

        previous_angle_to_travel = angle_to_travel;  // Stops if overshoots
        angle_to_travel = fabs(heading - tm->get_current_heading());
        if (angle_to_travel > 180) angle_to_travel = 360 - angle_to_travel;

    }

    hw->drivetrain.stop();                  
    vex::wait(35, vex::timeUnits::msec);  // Wait for odom wheels to update
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
    if (ISBACKTOPOSITION) distance_to_position = -distance_to_position;
        
    // move_drivetrain_distance({rc->auto_drive_velocity_percent, 0}, distance_to_position); // Drive at auto_drive_velocity_percent% velocity
    //changed to odometry function
    move_drivetrain_distance_odometry(position, ISBACKTOPOSITION);
}

