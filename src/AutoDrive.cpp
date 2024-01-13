#include "AutoDrive.h"


AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) {
        mp = new Map(telemetry, robotConfig, IS_SKILLS);
        pg = new PathGenerator(robotConfig, mp);
        //rc = RobotConfig;
    }

void AutoDrive::drive() {
    // Set braking
    std::cout << "START --------------" << std::endl;
    hw->left_drivetrain_motors.setStopping(vex::brakeType::brake);
    hw ->right_drivetrain_motors.setStopping(vex::brakeType::brake);
    hw->left_intake_expansion_motor.setStopping(vex::brakeType::hold);
    hw->right_intake_expansion_motor.setStopping(vex::brakeType::hold);

    std::vector<std::pair<double, double>> path;
    std::pair<double, double> curr_position = rc->starting_pos;
    std::pair<double, double> targ_position = {-36.0, -60.0};
    pg->generate_path(path, curr_position, targ_position);
    // path.push_back(curr_position);
    // path.push_back({-36.0, 35.0});
    // path.push_back({-37.0, 58.0});

    for (const std::pair<double, double> &pair : path) {
        std::cout << pair.first << " " << pair.second << '\n';
    }

    for (int i = 0; i < path.size() - 1; ++i) {
        std::cout << "Before move" << '\n';
        std::cout << "Path: (" << path.at(i).first << ", " << path.at(i).second << ") Next: ("<< path.at(i+1).first << ", " << path.at(i+1).second << ")" << std::endl;
        std::cout << "Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
        rotate_and_drive_to_position(path.at(i+1), false);
        std::cout << "After move" << '\n';
        std::cout << "Path: (" << path.at(i+1).first << ", " << path.at(i+1).second << std::endl;
        std::cout << "Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
        vex::wait(30, vex::timeUnits::msec);
    }
}

void AutoDrive::rotate_to_heading(double heading)
{
    std::cout << "Rotating to " << heading << std::endl;
    // Corrects heading to be from 0-360 from the x axis counterclockwise if applicable
    heading = fmod(heading, 360);
    if (heading < 0) heading += 360;
    double min_velocity = 20;
    double max_velocity = 50;
    const double stopping_aggression = 0.01; // Lower number is higher aggression (steeper slope)
    double velocity;

    // 1 if counterclockwise, -1 if clockwise depending on which direction is shorter to turn
    double turn_direction = ((fabs(heading - tm->get_current_heading()) > 180) 
        ^ (heading > tm->get_current_heading())) ? 1: -1;

    double angle_to_travel = fabs(heading - tm->get_current_heading());
    // Makes sure total angle to travel is shorter distance
    if (angle_to_travel > 180) angle_to_travel = 360 - angle_to_travel;
    double total_angle_to_travel = angle_to_travel;
    double previous_angle_to_travel = angle_to_travel + 1; // For not overshooting

    // Slow for small angles bc function doesn't slow things down enough.
    if (angle_to_travel < 30) {
        min_velocity = 20;
        max_velocity = 25;
    }
    std::cout << "Angle to travel: " << angle_to_travel << std::endl;

    // Turn wheels opposite of each other
    hw->left_drivetrain_motors.spin(vex::directionType::fwd, -min_velocity * turn_direction, 
        vex::velocityUnits::pct);
    hw->right_drivetrain_motors.spin(vex::directionType::fwd, min_velocity * turn_direction, 
    vex::velocityUnits::pct);
    std::cout << "Left velocity: " << hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct) << " Right velocity: " << hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct) << std::endl;
    vex::wait(30, vex::timeUnits::msec);    

    // Turn until within 1 degrees of desired heading or until it overshoots
    // (change in angle starts majorly increasing instead of decreasing)
    while (angle_to_travel > 1 && (previous_angle_to_travel - angle_to_travel) >= -0.05) { //TODO: Something isn't working here
    // while (fabs(tm->get_current_heading() - heading) > 1.0) { //! REMOVE OR REFACTOR
        std::cout << "    V: " << velocity << ", Angle to Travel: " << angle_to_travel << std::endl;
        std::cout << "    Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
        // Speeds up as leaving initial angle and slows down as approaching destination
        if (angle_to_travel > total_angle_to_travel/2) {
            // First half of distance
            velocity = atan(fabs(total_angle_to_travel - angle_to_travel)) * 2 
                * (max_velocity-min_velocity) / M_PI + min_velocity;
        } else {
            // Second half of distance
            velocity = atan(stopping_aggression * angle_to_travel) * 2 * max_velocity / M_PI;
        }


        hw->left_drivetrain_motors.setVelocity(-velocity * turn_direction, vex::velocityUnits::pct);
        hw->right_drivetrain_motors.setVelocity(velocity * turn_direction, vex::velocityUnits::pct);
        
        vex::wait(50, vex::timeUnits::msec);  // Wait for odometry to update
       
        previous_angle_to_travel = angle_to_travel;  // For stopping if overshoots
        angle_to_travel = fabs(heading - tm->get_current_heading());
        // Make sure to keep shortest angle to travel (just incase it passes 0/360)
        if (angle_to_travel > 180) angle_to_travel = 360 - angle_to_travel; 

    }
        std::cout << "    V: " << velocity << ", Angle to Travel: " << angle_to_travel << std::endl;
        std::cout << "    Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;


    hw->drivetrain.stop();                  
    vex::wait(1000, vex::timeUnits::msec);  // Wait for odometry wheels to update
}


void AutoDrive::rotate_to_position(std::pair<double, double> final_position, bool ISBACKROTATION)
{
    // if (IS_USING_GPS_POSITION) tm->set_current_heading(tm->getGPSPosition());
    double heading = tm->get_heading_between_points(tm->get_current_position(), final_position); // Gets absolute angle
    std::cout << "Intended Heading: " << heading << '\n';
    if (ISBACKROTATION)
        heading -= 180;
    rotate_to_heading(heading); 

}

void AutoDrive::rotate_and_drive_to_position(std::pair<double, double> position, 
    bool ISBACKTOPOSITION)
{

    rotate_to_position(position, ISBACKTOPOSITION);
    //vex::wait(1000,vex::timeUnits::msec); // TODO: TAKE THIS OUT

    // if (IS_USING_GPS_HEADING) tm->set_current_heading(tm->getGPSPosition());

    double distance_to_position = tm->get_distance_between_points(tm->get_current_position(), 
        position); // inches
    if (ISBACKTOPOSITION) distance_to_position = -distance_to_position;
        
    drive_to_position(position, ISBACKTOPOSITION);
    //vex::wait(1000,vex::timeUnits::msec); // TODO: TAKE THIS OUT
}

void AutoDrive::drive_to_position(std::pair<double, double> position, bool ISBACKTOPOSITION) 
{

    double current_distance = tm->get_distance_between_points(tm->get_current_position(), position);
    double distance = current_distance; // Distance goal    
    double previous_distance = current_distance; 

    const double min_velocity = 20;
    const double max_velocity = 80;
    const double stopping_aggression = 0.1; // Lower number is higher aggression (steeper slope)
    double velocity;

    // 1 if forward, -1 if backward
    double drive_direction = ISBACKTOPOSITION ? -1: 1;
    hw->drivetrain.spin(vex::directionType::fwd, min_velocity * drive_direction, 
        vex::velocityUnits::pct);
    std::cout << "Left velocity: " << hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct) << " Right velocity: " << hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct) << std::endl;
    vex::wait(30, vex::timeUnits::msec);
    
    std::cout << "Driving " << distance << " inches to (" << position.first << ", " << position.second << ")" << std::endl;

    // Turn until within 0.5 inches of desired distance or until it overshoots 
    // (change in distance starts majorly increasing instead of decreasing)
    while (fabs(current_distance) > 0.5 && (previous_distance - current_distance) >= -0.01) {
        // Speeds up as leaving initial position and slows down as approaching destination
        //std::cout << "      Current Distance: " << current_distance << " Current Heading: " << tm->get_current_heading() << std::endl;
        //std::cout << "    Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
        //std::cout << "    V: " << velocity << " Current Distance: " << current_distance << std::endl;
        if (current_distance >= distance/2) {
            // First half of distance
            velocity = atan(distance - current_distance) * 2 * (max_velocity-min_velocity) / M_PI 
                + min_velocity;
        } else {
            // Second half of distance
            velocity = atan(stopping_aggression * current_distance) * 2 * max_velocity / M_PI;
        }
        hw->drivetrain.setVelocity(velocity * drive_direction, vex::velocityUnits::pct);
      
        vex::wait(50, vex::timeUnits::msec);  // Wait for odometry wheels to update

        previous_distance = current_distance; // So don't overshoot
        current_distance = tm->get_distance_between_points(tm->get_current_position(), position); 
    }
        //std::cout << "      Current Distance: " << current_distance << " Current Heading: " << tm->get_current_heading() << std::endl;
    std::cout << "    Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
    std::cout << "    V: " << velocity << " Current Distance: " << current_distance << std::endl;

    hw->drivetrain.stop();  // Stop wheels
    vex::wait(1000, vex::timeUnits::msec);  // Wait for odometry wheels to update
}

// AutoDrive::get_point_with_offset(GameElement* element)
// {
//     InteractionObject* object = static_cast<InteractionObject*>(element);
//     std::pair<double, double> new_position = element->get_position();

//     if (object->get_interaction_angle == tm->get_current_heading())
//     {
//         return new_position;
//     }
    
//     // adjusted for heading_adjust, original interaction_angle = ...
//     // 0
//     if (object->interaction_angle == 90) { new_position.second - }
//     // 45
//     // 90
//     // 135
//     // 180
//     // 270
//     // -45
//     // -90



//     return new_position;
// }
