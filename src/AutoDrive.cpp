#include "AutoDrive.h"


AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) {
        mp = new Map(telemetry, robotConfig, IS_SKILLS);
        pg = new PathGenerator(robotConfig, mp, tm);
        rc = robotConfig;
    }

void AutoDrive::drive() {
    // Set braking
    std::cout << "START --------------" << std::endl;
    hw->left_drivetrain_motors.setStopping(vex::brakeType::brake);
    hw ->right_drivetrain_motors.setStopping(vex::brakeType::brake);
    hw->left_intake_expansion_motor.setStopping(vex::brakeType::hold);
    hw->right_intake_expansion_motor.setStopping(vex::brakeType::hold);

    // tm->set_position({0,0});
    // tm->set_heading(0);

    // run_catapult_catapult_strategy();
    // turbo_drive_distance(21.0, true);
    
    //test_turbo();
    // test_odometry();
    // std::cout << mp->buffers.at(0)->in_buffer({-47.0, 31.0}) << '\n';
    // std::cout << pg->path_is_clear(tm->get_current_position(), {-35.0, 58.0}) << '\n';
    path = pg->generate_path(tm->get_current_position(), {20.0, -20.0}); //TODO: Testing
    for (int i = 0; i < path.size(); ++i) {
        std::cout << "(" << path.at(i).first << ", " << path.at(i).second << ")" << '\n';
    }
    // execute_skills_plan(); //! ELIMINATE OPPONENTS
    // std::vector<std::pair<double, double>> path;
    // std::pair<double, double> curr_position = rc->starting_pos;
    // std::pair<double, double> targ_position = {-36.0, -60.0};
    // pg->generate_path(path, curr_position, targ_position);
    // // path.push_back(curr_position);
    // // path.push_back({-36.0, 35.0});
    // // path.push_back({-37.0, 58.0});

    // for (const std::pair<double, double> &pair : path) {
    //     std::cout << pair.first << " " << pair.second << '\n';
    // }

    // for (int i = 0; i < path.size() - 1; ++i) {
    //     std::cout << "Before move" << '\n';
    //     std::cout << "Path: (" << path.at(i).first << ", " << path.at(i).second << ") Next: ("<< path.at(i+1).first << ", " << path.at(i+1).second << ")" << std::endl;
    //     std::cout << "Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
    //     rotate_and_drive_to_position(path.at(i+1), false);
    //     std::cout << "After move" << '\n';
    //     std::cout << "Path: (" << path.at(i+1).first << ", " << path.at(i+1).second << std::endl;
    //     std::cout << "Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
    //     vex::wait(30, vex::timeUnits::msec);
    // }
}

void AutoDrive::test_odometry() {

    // Set initial heading/position
    tm->set_heading(0);
    tm->set_position({0,0});

    // while(true) {
    //     std::cout << "Odom: (" << tm->get_current_position().first << ", "  << tm->get_current_position().second << "),  " << tm->get_current_heading() << " deg" << std::endl;
    //     vex::wait(50, vex::timeUnits::msec);
    // }
   
    std::pair<double, double> position = {12,0};
    std::cout << "Moving to ("<< position.first << ", "  << position.second << "). Currently at " << tm->get_current_position().first << ", "  << tm->get_current_position().second << "),  " << tm->get_current_heading() << " deg" << std::endl;
    rotate_to_heading(90);

    std::cout << "Ending at (" << tm->get_current_position().first << ", "  << tm->get_current_position().second << "),  " << tm->get_current_heading() << " deg" << std::endl;

}

void AutoDrive::test_turbo() {

    // drive_to_position(mp->get_point_with_offset(mp->goals[0], false), true, true);
    rotate_and_drive_to_position(mp->goals[0], true, false, true);
}

void AutoDrive::execute_skills_plan() {

    if (rc->ROBOT == SCRAT) {

        // Launch match loads

        // So robot can turn
        drive_to_position(
            {
                rc->starting_pos.first + (rc->ACTUAL_WIDTH / 2.0),
                rc->starting_pos.second - (rc->ACTUAL_WIDTH / 2.0)
            },
            true
        );
        for (int i = 0; i < 4; ++i) run_plow_strategy(); // Until end of match
    }

    else {
        // SCRATETTE auto skills plan
        drive_to_position(
            {
                rc->starting_pos.first + (rc->ACTUAL_WIDTH / 2.0),
                rc->starting_pos.second + (rc->ACTUAL_WIDTH / 2.0)
            },
            true
        );
    }
}

void AutoDrive::drive_along_path() {
    for (int i = 0; i < path.size() - 1; ++i) {
        if (i < path.size() - 2) {
            if (!pg->path_is_clear(tm->get_current_position(), path.at(i+1))) {
                std::vector<std::pair<double, double>> sub_path = pg->generate_path(
                    tm->get_current_position(), path.at(i+1)
                );
                sub_path.erase(sub_path.begin());
                sub_path.erase(sub_path.end());
                for (int j = sub_path.size() - 1; j >= 0; --j) {
                    path.insert(path.begin() + i, sub_path.at(j));
                }
            }
        }
        // std::cout << "Before move" << '\n';
        // std::cout << "Path: (" << path.at(i).first << ", " << path.at(i).second << ") Next: ("<< path.at(i+1).first << ", " << path.at(i+1).second << ")" << std::endl;
        // std::cout << "Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
        rotate_and_drive_to_position(path.at(i+1), false);
        // std::cout << "After move" << '\n';
        // std::cout << "Path: (" << path.at(i+1).first << ", " << path.at(i+1).second << std::endl;
        // std::cout << "Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
        vex::wait(50, vex::timeUnits::msec);
    }
}

void AutoDrive::pathfind_and_drive_to_position(std::pair<double, double> target_position) {
    path = pg->generate_path(tm->get_current_position(), target_position);
    drive_along_path();
}

void AutoDrive::rotate_to_heading(double heading, bool IS_TURBO)
{
    // std::cout << "Rotating to " << heading << std::endl;
    // Corrects heading to be from 0-360 from the x axis counterclockwise if applicable
    if (IS_TURBO) {
        turbo_turn(heading);
        return;
    }

    heading = fmod(heading, 360);
    if (heading < 0) heading += 360;
    double min_velocity = 20;
    double max_velocity = 50;
    double stopping_aggression = 0.015; // Lower number is higher aggression (steeper slope)

    // Scratette will go faster bc she has bigger wheels
    if (rc->ROBOT == SCRATETTE) {
        min_velocity = 10;
        max_velocity = 30;
        stopping_aggression = 0.03;
    }

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
    if (angle_to_travel < 35) {
        min_velocity = 20;
        max_velocity = 25;
        
        // Scratette will go faster bc she has bigger wheels
        if (rc->ROBOT == SCRATETTE) {
            min_velocity = 10;
            max_velocity = 15;
            stopping_aggression = 0.2;
        }
    }
    // std::cout << "Angle to travel: " << angle_to_travel << std::endl;

    // Turn wheels opposite of each other
    hw->left_drivetrain_motors.spin(vex::directionType::fwd, -min_velocity * turn_direction, 
        vex::velocityUnits::pct);
    hw->right_drivetrain_motors.spin(vex::directionType::fwd, min_velocity * turn_direction, 
    vex::velocityUnits::pct);
    // std::cout << "Left velocity: " << hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct) << " Right velocity: " << hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct) << std::endl;
    vex::wait(30, vex::timeUnits::msec);    

    // Turn until within 1 degrees of desired heading or until it overshoots
    // (change in angle starts majorly increasing instead of decreasing)
    while (angle_to_travel > 1 && (previous_angle_to_travel - angle_to_travel) >= -0.05) { 
    // while (fabs(tm->get_current_heading() - heading) > 1.0) { //! REMOVE OR REFACTOR
        // std::cout << "    V: " << velocity << ", Angle to Travel: " << angle_to_travel << std::endl;
        //std::cout << "    Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
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
        // std::cout << "    V: " << velocity << ", Angle to Travel: " << angle_to_travel << std::endl;
        // std::cout << "    Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
    std::cout << "Done turning\n";

    hw->drivetrain.stop();                  
    vex::wait(1000, vex::timeUnits::msec);  // Wait for odometry wheels to update
}


void AutoDrive::rotate_to_position(std::pair<double, double> final_position, bool ISBACKROTATION, 
    bool IS_TURBO)
{
    // if (IS_USING_GPS_POSITION) tm->set_current_heading(tm->getGPSPosition());
    double heading = tm->get_heading_between_points(tm->get_current_position(), final_position); // Gets absolute angle
    std::cout << "Intended Heading: " << heading << '\n';
    if (ISBACKROTATION)
        heading -= 180;
    rotate_to_heading(heading, IS_TURBO); 

}

void AutoDrive::rotate_to_position(InteractionObject *element, bool IS_BACK_POSITION,
    bool IS_OFFSET, bool IS_OFFSET_EXTRA, bool IS_TURBO)
{
    std::pair<double, double> position = {};
    if (IS_OFFSET || IS_OFFSET_EXTRA) {
        position = mp->get_point_with_offset(element, IS_OFFSET_EXTRA);
    } else position = element->get_position();

    rotate_to_position(position, IS_BACK_POSITION, IS_TURBO);
}

void AutoDrive::rotate_and_drive_to_position(std::pair<double, double> position, 
    bool ISBACKTOPOSITION, bool IS_TURBO)
{

    rotate_to_position(position, ISBACKTOPOSITION, IS_TURBO);

    double distance_to_position = tm->get_distance_between_points(tm->get_current_position(), 
        position); // inches
    if (ISBACKTOPOSITION) distance_to_position = -distance_to_position;
        
    drive_to_position(position, ISBACKTOPOSITION);

}

void AutoDrive::rotate_and_drive_to_position(InteractionObject *element, bool IS_BACK_POSITION, 
    bool IS_OFFSET, bool IS_OFFSET_EXTRA, bool IS_TURBO) {
    
    std::pair<double, double> position = {};
    if (IS_OFFSET || IS_OFFSET_EXTRA) {
        position = mp->get_point_with_offset(element, IS_OFFSET_EXTRA);
    } else position = element->get_position();
    rotate_and_drive_to_position(position, IS_BACK_POSITION, IS_TURBO);
}

void AutoDrive::drive_to_position(std::pair<double, double> position, bool ISBACKTOPOSITION,
    bool IS_TURBO) 
{
    std::cout << "Driving. . ." << '\n';

    double current_distance = tm->get_distance_between_points(tm->get_current_position(), position);
    double distance = current_distance; // Distance goal    
    double previous_distance = current_distance; 

    if (IS_TURBO) {
        turbo_drive_distance(distance, ISBACKTOPOSITION);
        return;
    }

    double min_velocity = 20;
    double max_velocity = 80;

    // Scratette will go faster bc she has bigger wheels
    if (rc->ROBOT == SCRATETTE) {
        min_velocity = 10;
        max_velocity = 50;
    }
    const double stopping_aggression = 0.1; // Lower number is higher aggression (steeper slope)
    double velocity;

    // 1 if forward, -1 if backward
    double drive_direction = ISBACKTOPOSITION ? -1: 1;
    hw->drivetrain.spin(vex::directionType::fwd, min_velocity * drive_direction, 
        vex::velocityUnits::pct);
    // std::cout << "Left velocity: " << hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct) << " Right velocity: " << hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct) << std::endl;
    vex::wait(30, vex::timeUnits::msec);
    
    // std::cout << "Driving " << distance << " inches to (" << position.first << ", " << position.second << ")" << std::endl;

    // Turn until within 0.5 inches of desired distance or until it overshoots 
    // (change in distance starts majorly increasing instead of decreasing)
    while (fabs(current_distance) > 0.5 && (previous_distance - current_distance) >= -0.01) {
        // Speeds up as leaving initial position and slows down as approaching destination
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
    // std::cout << "    Odemetry: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << ") " << tm->get_current_heading() << std::endl;
    // std::cout << "    V: " << velocity << " Current Distance: " << current_distance << std::endl;

    hw->drivetrain.stop();  // Stop wheels
    vex::wait(1000, vex::timeUnits::msec);  // Wait for odometry wheels to update
}
void AutoDrive::turbo_drive_distance(double distance, bool IS_REVERSE) {

    std::cout << rc->WHEEL_CIRCUMFERENCE << '\n';
    double num_wheel_revolutions = distance / rc->WHEEL_CIRCUMFERENCE;
    std::cout << num_wheel_revolutions << '\n';
    
    // std::pair<double, double> vel = calculateDriveTrainVel(velPercent);

    hw->left_drivetrain_motors.resetPosition();
    hw->right_drivetrain_motors.resetPosition();

    if (IS_REVERSE) {
        hw->left_drivetrain_motors.spinTo(-num_wheel_revolutions, vex::rotationUnits::rev, 80.0, vex::velocityUnits::pct, false);
        hw->right_drivetrain_motors.spinTo(-num_wheel_revolutions, vex::rotationUnits::rev, 80.0, vex::velocityUnits::pct, false);
    }
    else {
        hw->left_drivetrain_motors.spinTo(num_wheel_revolutions, vex::rotationUnits::rev, 80.0, vex::velocityUnits::pct, false);
        hw->right_drivetrain_motors.spinTo(num_wheel_revolutions, vex::rotationUnits::rev, 80.0, vex::velocityUnits::pct, false);
    }
    vex::wait(50, vex::timeUnits::msec);
    while(fabs(hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0.0 || fabs(hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0.0); //Blocks other tasks from starting 

    std::cout << "Turbo done\n";

}

void AutoDrive::turbo_turn(double heading)
{
    // Corrects heading to be from 0-360 from the x axis counterclockwise if applicable
    heading = fmod(heading, 360);
    if (heading < 0)
        heading += 360;

    double angle_to_rotate = heading - tm->get_current_heading();
    angle_to_rotate = fmod(angle_to_rotate, 360); // make sure the angle to rotate is -360 to 360

    // Determines whether to rotate left or right based on the  shortest distance
    if (360 - fabs(angle_to_rotate) < angle_to_rotate)
        angle_to_rotate = angle_to_rotate - 360;
    
    double revolutions = angle_to_rotate  * (rc->DRIVETRAIN_WIDTH) * M_PI 
        / (360 * rc->WHEEL_CIRCUMFERENCE);

    hw->left_drivetrain_motors.resetPosition();
    hw->right_drivetrain_motors.resetPosition();

    double velocity = 80;

    hw->left_drivetrain_motors.spinFor(-revolutions, vex::rotationUnits::rev, velocity, 
        vex::velocityUnits::pct, false);
    hw->right_drivetrain_motors.spinFor(revolutions, vex::rotationUnits::rev, velocity, 
        vex::velocityUnits::pct);

    // Blocks other tasks from starting
    while (fabs(hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0 
        || fabs(hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0); 

}

void AutoDrive::climb_distance(double height) {
    double climb_wheel_circumference = rc->CLIMB_WHEEL_DIAMETER * M_PI;
    double climb_wheel_revolution = height / climb_wheel_circumference;
    double velocity = 100;  // TODO: change later

    hw->climb_motors.spinFor(climb_wheel_revolution, vex::rotationUnits::rev, velocity,
        vex::velocityUnits::pct);
}



void AutoDrive::run_plow_strategy() {

    std::pair<double, double> prep_pos;
    std::pair<double, double> target_pos;

    // Ram at top of red goal
    target_pos = mp->goals[3]->get_position();
    prep_pos.first = 70.02 - 14.0;
    prep_pos.second = target_pos.second + 21.0; // Offset from goal by almost 2 feet
    // target_pos.second += rc->DRIVETRAIN_RADIUS - 2;
    pathfind_and_drive_to_position(prep_pos);
    this->rotate_to_heading(90.0);
    // vex::wait(1000, vex::timeUnits::msec);
    if (!SNOWPLOW_OUT) snowplow_out();
    
    // drive_to_position(target_pos, true);
    // turbo_drive_distance(tm->get_distance_between_points(
    //     tm->get_current_position(),
    //     mp->get_point_with_offset(mp->goals[3], false)
    // ), true);
    // drive_to_position(mp->get_point_with_offset(mp->goals[3], false), true, true);
    // rotate_and_drive_to_position(mp->goals[3], true, false, true);
    // drive_to_position(mp->get_point_with_offset(mp->goals[3], false), true, true);
    std::cout << target_pos.first << " " << target_pos.second << " " << tm->get_distance_between_points(tm->get_current_position(), target_pos) << '\n';
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), target_pos), true
    );
    vex::wait(500, vex::timeUnits::msec);
    // vex::wait(50000, vex::timeUnits::msec);
    // snowplow_in();
    // drive_to_position(prep_pos, true);
    // turbo_drive_distance(21.0, false);
    // drive_to_position(prep_pos, false, true);
    // rotate_and_drive_to_position(prep_pos, false, true);
    // drive_to_position(prep_pos, false, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), prep_pos), false
    );
    // Ram at side-top of red goal
    // snowplow_out();
    std::cout << "1" << '\n';
    target_pos = mp->goals[0]->get_position();
    prep_pos = target_pos;
    std::cout << "2" << '\n';
    prep_pos.first -= 31.0;
    target_pos.first += rc->DRIVETRAIN_RADIUS - 2;
    std::cout << "3" << '\n';
    pathfind_and_drive_to_position(prep_pos);
    this->rotate_to_heading(180.0);
    // drive_to_position(target_pos, true);
    // turbo_drive_distance(31.0, true);
    // drive_to_position(mp->get_point_with_offset(mp->goals[0], false), true, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), target_pos), true
    );
    vex::wait(500, vex::timeUnits::msec);
    // snowplow_in();
    // drive_to_position(prep_pos, true);
    // turbo_drive_distance(31.0, false);
    // drive_to_position(prep_pos, false, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), prep_pos), false
    );

    // Ram at side-bottom of red goal
    // snowplow_out();
    target_pos = mp->goals[2]->get_position();
    prep_pos = target_pos;
    prep_pos.first -= 31.0;
    target_pos.first += rc->DRIVETRAIN_RADIUS - 2;
    pathfind_and_drive_to_position(prep_pos);
    this->rotate_to_heading(180.0);
    // drive_to_position(target_pos, true);
    // turbo_drive_distance(31.0, true);
    // rotate_and_drive_to_position(mp->goals[2], true, false, true);
    // drive_to_position(mp->get_point_with_offset(mp->goals[2], false), true, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), target_pos), true
    );
    vex::wait(500, vex::timeUnits::msec);
    // snowplow_in();
    // drive_to_position(prep_pos, true);
    // turbo_drive_distance(31.0, false);
    // drive_to_position(prep_pos, false, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), prep_pos), false
    );

    // Ram at bottom of red goal
    // snowplow_out();
    target_pos = mp->goals[4]->get_position();
    prep_pos.first = 70.02 - 14.0;
    prep_pos.second = target_pos.second - 21.0; // Offset from goal by almost 2 feet
    target_pos.second -= rc->DRIVETRAIN_RADIUS - 2;
    pathfind_and_drive_to_position(prep_pos);
    rotate_to_heading(270.0);
    // drive_to_position(target_pos, true);
    // turbo_drive_distance(tm->get_distance_between_points(
    //     tm->get_current_position(),
    //     mp->get_point_with_offset(mp->goals[4], false)
    // ), true);
    // rotate_and_drive_to_position(mp->goals[4], true, false, true);
    // drive_to_position(mp->get_point_with_offset(mp->goals[4], false), true, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), target_pos), true
    );
    vex::wait(500, vex::timeUnits::msec);
    // snowplow_in();
    // drive_to_position(prep_pos, true);
    // turbo_drive_distance(21.0, false);
    // drive_to_position(prep_pos, false, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), prep_pos), false
    );

    // Returning direction -------------------------------------------------------------------------

    // Ram at side-bottom of red goal
    // snowplow_out();
    target_pos = mp->goals[2]->get_position();
    prep_pos = target_pos;
    prep_pos.first -= 31.0;
    target_pos.first += rc->DRIVETRAIN_RADIUS - 2 ;
    pathfind_and_drive_to_position(prep_pos);
    rotate_to_heading(0.0);
    // drive_to_position(target_pos, false);
    // turbo_drive_distance(31.0, true);
    // rotate_and_drive_to_position(mp->goals[2], true, false, true);
    // drive_to_position(mp->get_point_with_offset(mp->goals[2], false), true, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), target_pos), true
    );
    vex::wait(500, vex::timeUnits::msec);
    // snowplow_in();
    // drive_to_position(prep_pos, true);
    // turbo_drive_distance(31.0, false);
    // drive_to_position(prep_pos, false, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), prep_pos), false
    );

    // Ram at side-top of red goal
    // snowplow_out();
    target_pos = mp->goals[0]->get_position();
    prep_pos = target_pos;
    prep_pos.first -= 31.0;
    target_pos.first += rc->DRIVETRAIN_RADIUS;
    pathfind_and_drive_to_position(prep_pos);
    rotate_to_heading(0.0);
    // drive_to_position(target_pos, false);
    // turbo_drive_distance(31.0, true);
    // rotate_and_drive_to_position(mp->goals[0], true, false, true);
    // drive_to_position(mp->get_point_with_offset(mp->goals[0], false), true, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), target_pos), true
    );
    vex::wait(500, vex::timeUnits::msec);
    // snowplow_in();
    // drive_to_position(prep_pos, true);
    // turbo_drive_distance(31.0, false);
    // drive_to_position(prep_pos, false, true);
    turbo_drive_distance(tm->get_distance_between_points(
        tm->get_current_position(), prep_pos), false
    );

    //? May be covered already
    // Ram at top of red goal
    //// snowplow_out();
    // target_pos = mp->goals[3]->get_position();
    // prep_pos = target_pos;
    // prep_pos.second += 18.0; // Offset from goal by 1.5 feet
    // target_pos.second += rc->DRIVETRAIN_RADIUS;
    // path = pg->generate_path(tm->get_current_position(), prep_pos);
    // drive_along_path();
    // rotate_to_heading(-90.0);
    // drive_to_position(target_pos, false);
    // vex::wait(500, vex::timeUnits::msec);
    //// snowplow_in();
    // drive_to_position(prep_pos, true);
}

void AutoDrive::run_catapult_catapult_strategy() {
    // TODO change initial location?
    std::pair<double, double> goal_position;
    double goal_heading;

    const int NUMBER_TRIBALLS = 3;
    if (rc->ROBOT == SCRAT) {

    } else { // SCRATETTE

        //TODO modify this
        goal_heading = mp->loadzones[1]->get_interaction_angle();
        tm->set_heading(goal_heading);

        std::cout << "Heading: " << tm->get_current_heading() << std::endl;
        goal_position = mp->get_point_with_offset(mp->loadzones[1], false);
        tm->set_position(goal_position);
        std::cout << "Position: " << tm->get_current_position().first << "," << tm->get_current_position().second << std::endl;

        expand_intake();
        vex::wait(2, vex::timeUnits::sec);
        stop_intake_expansion();
        //activate_intake(); // TODO uncomment
        //while(1);

        for (int i = 0; i < NUMBER_TRIBALLS; i++) {
            std::cout << i << std::endl;
            std::cout << "  Heading: " << tm->get_current_heading() << std::endl;
            std::cout << "  Position: " << tm->get_current_position().first << "," << tm->get_current_position().second << std::endl;
            // Lower red offset
            rotate_and_drive_to_position(mp->loadzones[1], true, false, true, true); 
            //while(1);
            std::cout << "  Heading: " << tm->get_current_heading() << std::endl;
            std::cout << "  Position: " << tm->get_current_position().first << "," << tm->get_current_position().second << std::endl;
            rotate_to_position(mp->goals[2], true, false, false, true);
            vex::wait(1, vex::timeUnits::sec);
            //launch_catapult();
            std::cout << "  Heading: " << tm->get_current_heading() << std::endl;
            std::cout << "  Position: " << tm->get_current_position().first << "," << tm->get_current_position().second << std::endl;
            rotate_and_drive_to_position(mp->loadzones[1], false, true, false, true); 
            std::cout << "  Heading: " << tm->get_current_heading() << std::endl;
            std::cout << "  Position: " << tm->get_current_position().first << "," << tm->get_current_position().second << std::endl;
        }

        stop_intake();
        retract_intake();
        vex::wait(2, vex::timeUnits::sec);
        stop_intake_expansion();
    }

}
