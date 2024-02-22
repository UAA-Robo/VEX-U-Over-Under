#include "AutoDrive.h"

AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry)
{
    mp = new Map(telemetry, robotConfig, IS_SKILLS);
    pg = new PathGenerator(robotConfig, mp, tm);
    rc = robotConfig;
}

void AutoDrive::drive()
{
    // Set braking
    std::cout << "START --------------" << std::endl;
    hw->left_drivetrain_motors.setStopping(vex::brakeType::brake);
    hw->right_drivetrain_motors.setStopping(vex::brakeType::brake);
    hw->left_intake_expansion_motor.setStopping(vex::brakeType::hold);
    hw->right_intake_expansion_motor.setStopping(vex::brakeType::hold);

    // tm->set_heading(0);
    // tm->set_position({0,0});
    // //rotate_to_heading(45);
    // drive_to_position({24, 0});
    // std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

    execute_head_to_head_plan(); //! ELIMINATE OPPONENTS
}

void AutoDrive::execute_head_to_head_plan()
{

    if (rc->ROBOT == SCRAT)
    {
        tm->set_position({-17.0, -61});
        tm->set_heading(180.0);

        // Retract catapult
        expand_intake();
        vex::wait(800, vex::timeUnits::msec);
        stop_intake_expansion();
        vex::task catapult_task = vex::task(run_catapult_thread, this, 1);
        vex::wait(300, vex::timeUnits::msec);
        retract_intake();
        vex::wait(300, vex::timeUnits::msec);
        stop_intake_expansion();

        std::pair<double, double> target = {43, -61}; //{46, -61}

        // Drive to loading zone and turn parrallel
        drive_to_position(target, true);
        // 1
        std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;
        turbo_turn(200.0);

        // Sweep triball out
        left_snowplow_out();
        turbo_drive_distance(13.5, true, 50.0);
        turbo_turn(270.0, 50.0);
        left_snowplow_in();
        turbo_turn(225.0, 50.0);
        turbo_drive_distance(8.0, true, 50.0);
        turbo_turn(270.0);
        turbo_drive_distance(14.0, true, 70.0);
        vex::wait(300, vex::timeUnits::msec);
        // 2
        std::cout << "2 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        tm->set_heading(270);
        tm->set_position({61, -31});

        // 3.5
        std::cout << "3.5 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        // Sweep other triballs into net
        // turbo_drive_distance(10.0, false, 40.0);
        drive_to_position({61, -41}, false);
        // 3
        std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        this->turbo_turn_velocity = 20;
        this->turbo_drive_velocity = 30;

        // rotate_and_drive_to_position({34.98, -34.98}, true);

        rotate_to_position({34.98, -34.98}, true, false);
        drive_to_position({34.98, -34.98}, true);
        // 4
        std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        rotate_to_position({34.98, -24}, true, false);
        right_snowplow_out();
        drive_to_position({34.98, -24}, true);
        // 5
        std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        right_snowplow_in();
        rotate_to_position({16, -20}, true, false);
        drive_to_position({16, -20}, true);
        vex::wait(50, vex::timeUnits::msec);

        // 6
        std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;
        rotate_to_position({16, -10}, true, false);
        drive_to_position({16, -10}, true);
        // 7
        std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        // drive into  goal
        right_snowplow_out();
        // rotate_to_heading(180);
        rotate_to_position({52, -10}, true);
        left_snowplow_out();
        turbo_drive_distance(50, true, 50.0);

        // 8
        std::cout << " 8 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;
        // drive_to_position({52, 0}, true);
        snowplow_in();

        float y_position = tm->get_current_position().second;
        tm->set_heading(180);
        tm->set_position({40, y_position});

        // 8.5
        std::cout << " 8.5 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        // Go to bar
        drive_to_position({30, y_position});
        // 9
        std::cout << " 9 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;
        // turbo_drive_distance(5, false, 30.0); // back up from goal

        // 10
        std::cout << " 10 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;
        rotate_and_drive_to_position({7, -39});
        // rotate_to_heading(240);
        // turbo_drive_distance(44, false, 30);

        // 10.5
        // std::cout << "10.5 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;
        //
        // rotate_and_drive_to_position({6, -42});

        // 11
        std::cout << "11 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        turbo_drive_distance(5, false, 50); //

        expand_intake();
        vex::wait(1000, vex::timeUnits::msec);
        stop_intake_expansion();
        turbo_drive_distance(12, false, 50);

        std::cout << "Done!\n";
    }
    else
    {
        // SCRATETTE auto skills plan
        run_catapult_strategy(12);

        // Go to barrier and touch it
        turbo_drive_distance(8, true, 50);
        turbo_turn_relative(45, 30);
        turbo_drive_distance(14, false, 50);
        turbo_turn_relative(90, 30);
        turbo_drive_distance(40, false, 50);
    }
}
void AutoDrive::execute_skills_plan()
{

    if (rc->ROBOT == SCRAT)
    {
        tm->set_position({-53, -53});
        tm->set_heading(225);

        // 1
        std::cout << "1 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        run_catapult_strategy(7, true);

        // 2
        std::cout << "2 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        // // Estimate of where we are
        // tm->set_position({-50.5, -48});
        // tm->set_heading(225);

        retract_intake();
        vex::wait(500, vex::timeUnits::msec);
        stop_intake_expansion();

        rotate_and_drive_to_position({-17.0, -61}, true);
        // 3
        std::cout << "3 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;
        rotate_to_position({46, -61}, true);

        // 4
        std::cout << "4 position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

        run_plow_strategy(); // Until end of match
    }
    else
    {
        // Don't need to set position/angle bc everything is relative
        // SCRATETTE auto skills plan
        run_catapult_strategy(30, false);
    }
}

void AutoDrive::drive_along_path()
{
    for (int i = 0; i < path.size() - 1; ++i)
    {

        rotate_and_drive_to_position(path.at(i + 1), false);
        vex::wait(50, vex::timeUnits::msec);
    }
}

void AutoDrive::pathfind_and_drive_to_position(std::pair<double, double> target_position)
{
    path.clear();
    path = pg->generate_path(tm->get_current_position(), target_position);
    drive_along_path();
}

void AutoDrive::rotate_to_heading(double heading, bool IS_TURBO)
{

    heading = fmod(heading, 360);
    if (heading < 0)
        heading += 360;

    std::cout << "HERE POSITION GOAL for: " << heading << std::endl;

    if (IS_TURBO)
    {
        turbo_turn(heading, this->turbo_turn_velocity);
        return;
    }

    double velocity;
    double min_velocity = this->min_turn_velocity;
    double max_velocity = max_turn_velocity;
    double stopping_aggression = turn_stopping_aggression;

    // 1 if counterclockwise, -1 if clockwise depending on which direction is shorter to turn
    double turn_direction = ((fabs(heading - tm->get_current_heading()) > 180) ^ (heading > tm->get_current_heading())) ? 1 : -1;

    double angle_to_travel = fabs(heading - tm->get_current_heading());
    // Makes sure total angle to travel is shorter distance
    if (angle_to_travel > 180)
        angle_to_travel = 360 - angle_to_travel;
    double total_angle_to_travel = angle_to_travel;
    double previous_angle_to_travel = angle_to_travel + 1; // For not overshooting

    // Slow for small angles bc function doesn't slow things down enough.
    if (angle_to_travel <= 63)
    {
        // min_velocity = 6;
        max_velocity = 8;
        stopping_aggression = 0.02;

        // Scratette will go faster bc she has bigger wheels
        // if (rc->ROBOT == SCRATETTE) {
        //     min_velocity = 10;
        //     max_velocity = 15;
        //     stopping_aggression = 0.2;
        // }
    }
    // std::cout << "Angle to travel: " << angle_to_travel << std::endl;

    // Turn wheels opposite of each other
    hw->left_drivetrain_motors.spin(vex::directionType::fwd, -min_velocity * turn_direction,
                                    vex::velocityUnits::pct);
    hw->right_drivetrain_motors.spin(vex::directionType::fwd, min_velocity * turn_direction,
                                     vex::velocityUnits::pct);
    // std::cout << "Left velocity: " << hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct) << " Right velocity: " << hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct) << std::endl;
    vex::wait(100, vex::timeUnits::msec);

    // Turn until within 1 degrees of desired heading or until it overshoots
    // (change in angle starts majorly increasing instead of decreasing)

    const double INITIAL_TIME = hw->brain.timer(vex::timeUnits::msec);
    const double TIMEOUT = 4000; // 4.0 seconds

    while (angle_to_travel > 1 && (hw->brain.timer(vex::timeUnits::msec) - INITIAL_TIME) < TIMEOUT && (previous_angle_to_travel - angle_to_travel) > -0.09)
    {

        // std::cout << tm->get_current_heading() << '\n';
        // std::cout << "(" << tm->get_current_position().first << ", " << tm->get_current_position().second << ")" << '\n';

        if (angle_to_travel > total_angle_to_travel / 2)
        {
            // First half of distance
            velocity = atan(fabs(total_angle_to_travel - angle_to_travel)) * 2 * (max_velocity - min_velocity) / M_PI + min_velocity;
        }
        else
        {
            // Second half of distance
            velocity = atan(stopping_aggression * angle_to_travel) * 2 * max_velocity / M_PI;
        }

        hw->left_drivetrain_motors.setVelocity(-velocity * turn_direction, vex::velocityUnits::pct);
        hw->right_drivetrain_motors.setVelocity(velocity * turn_direction, vex::velocityUnits::pct);

        vex::wait(50, vex::timeUnits::msec); // Wait for odometry to update

        previous_angle_to_travel = angle_to_travel; // For stopping if overshoots
        angle_to_travel = fabs(heading - tm->get_current_heading());
        // Make sure to keep shortest angle to travel (just incase it passes 0/360)
        if (angle_to_travel > 180)
            angle_to_travel = 360 - angle_to_travel;
    }

    hw->drivetrain.stop();
    vex::wait(500, vex::timeUnits::msec); // Wait for odometry wheels to update
}

void AutoDrive::rotate_to_position(std::pair<double, double> final_position, bool ISBACKROTATION,
                                   bool IS_TURBO)
{
    // if (IS_USING_GPS_POSITION) tm->set_current_heading(tm->getGPSPosition());
    double heading = tm->get_heading_between_points(tm->get_current_position(), final_position); // Gets absolute angle

    if (ISBACKROTATION)
    {
        heading -= 180;
    }
    std::cout << "HEADING GOAL for " << final_position.first << ", " << final_position.second << ": " << heading << std::endl;

    rotate_to_heading(heading, IS_TURBO);
}

void AutoDrive::rotate_to_position(InteractionObject *element, bool IS_BACK_POSITION,
                                   bool IS_OFFSET, bool IS_OFFSET_EXTRA, bool IS_TURBO)
{
    std::pair<double, double> position = {};
    if (IS_OFFSET || IS_OFFSET_EXTRA)
    {
        position = mp->get_point_with_offset(element, IS_OFFSET_EXTRA);
    }
    else
        position = element->get_position();

    rotate_to_position(position, IS_BACK_POSITION, IS_TURBO);
}

void AutoDrive::rotate_and_drive_to_position(std::pair<double, double> position,
                                             bool ISBACKTOPOSITION, bool IS_TURBO)
{

    rotate_to_position(position, ISBACKTOPOSITION, IS_TURBO);

    double distance_to_position = tm->get_distance_between_points(tm->get_current_position(),
                                                                  position); // inches
    if (ISBACKTOPOSITION)
        distance_to_position = -distance_to_position;

    drive_to_position(position, ISBACKTOPOSITION, IS_TURBO);
}

void AutoDrive::rotate_and_drive_to_position(InteractionObject *element, bool IS_BACK_POSITION,
                                             bool IS_OFFSET, bool IS_OFFSET_EXTRA, bool IS_TURBO)
{

    std::pair<double, double> position = {};
    if (IS_OFFSET || IS_OFFSET_EXTRA)
    {
        position = mp->get_point_with_offset(element, IS_OFFSET_EXTRA);
    }
    else
        position = element->get_position();
    rotate_and_drive_to_position(position, IS_BACK_POSITION, IS_TURBO);
}

void AutoDrive::drive_to_position(
    std::pair<double, double> position, bool ISBACKTOPOSITION, bool IS_TURBO)
{

    double current_distance = tm->get_distance_between_points(tm->get_current_position(), position);
    double distance = current_distance; // Distance goal
    double previous_distance = current_distance;

    if (IS_TURBO)
    {
        turbo_drive_distance(distance, ISBACKTOPOSITION, this->turbo_drive_velocity);
        return;
    }

    double velocity;

    // 1 if forward, -1 if backward
    double drive_direction = ISBACKTOPOSITION ? -1 : 1;
    hw->drivetrain.spin(vex::directionType::fwd, this->min_drive_velocity * drive_direction,
                        vex::velocityUnits::pct);
    // std::cout << "Left velocity: " << hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct) << " Right velocity: " << hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct) << std::endl;
    vex::wait(100, vex::timeUnits::msec);

    // Turn until within 0.5 inches of desired distance or until it overshoots
    // (change in distance starts majorly increasing instead of decreasing)
    while (fabs(current_distance) > 0.5 && (previous_distance - current_distance) >= -0.01)
    {
        // std::cout << "(" << tm->get_current_position().first << ", " << tm->get_current_position().second << ")\n";

        // Speeds up as leaving initial position and slows down as approaching destination
        if (current_distance >= distance / 2)
        {
            // First half of distance
            velocity = atan(distance - current_distance) * 2 * (this->max_drive_velocity - this->min_drive_velocity) / M_PI + this->min_drive_velocity;
        }
        else
        {
            // Second half of distance
            velocity = atan(this->drive_stopping_aggression * current_distance) * 2 * this->max_drive_velocity / M_PI;
        }
        hw->drivetrain.setVelocity(velocity * drive_direction, vex::velocityUnits::pct);

        vex::wait(50, vex::timeUnits::msec); // Wait for odometry wheels to update

        previous_distance = current_distance; // So don't overshoot
        current_distance = tm->get_distance_between_points(tm->get_current_position(), position);
    }

    hw->drivetrain.stop(); // Stop wheels
    std::cout << "HERE" << std::endl;
    vex::wait(500, vex::timeUnits::msec); // Wait for odometry wheels to update
}

// void AutoDrive::climb_distance(double height) {
//     double climb_wheel_circumference = rc->CLIMB_WHEEL_DIAMETER * M_PI;
//     double climb_wheel_revolution = height / climb_wheel_circumference;
//     double velocity = 100;  // TODO: change later

//     hw->climb_motors.spinFor(climb_wheel_revolution, vex::rotationUnits::rev, velocity,
//         vex::velocityUnits::pct);
// }

void AutoDrive::run_plow_strategy()
{
    // Assumes start at {-17.0, -61}, 180 deg

    std::pair<double, double> target = {46, -61};
    // Drive to loading zone and turn parrallel
    drive_to_position(target, true);
    // 1
    std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;
    turbo_turn(200.0);

    // Sweep triball out
    left_snowplow_out();
    turbo_drive_distance(12.0, true, 50.0);
    turbo_turn(270.0, 50.0);
    left_snowplow_in();
    turbo_turn(225.0, 50.0);
    turbo_drive_distance(8.0, true, 50.0);
    turbo_turn(270.0);
    turbo_drive_distance(14.0, true, 50.0);
    // 2
    std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

    tm->set_heading(270);
    tm->set_position({61, -31});

    // Sweep other triballs into net
    // turbo_drive_distance(10.0, false, 40.0);
    drive_to_position({61, -41}, false);
    // 3
    std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

    this->turbo_turn_velocity = 20;
    this->turbo_drive_velocity = 30;

    // rotate_and_drive_to_position({34.98, -34.98}, true);

    rotate_to_position({34.98, -34.98}, true, false);
    drive_to_position({34.98, -34.98}, true);
    // 4
    std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

    rotate_to_position({34.98, -24}, true, false);
    right_snowplow_out();
    drive_to_position({34.98, -24}, true);
    // 5
    std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

    right_snowplow_in();
    rotate_to_position({16, -20}, true, false);
    drive_to_position({16, -20}, true);
    vex::wait(50, vex::timeUnits::msec);

    // 6
    std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;
    rotate_to_position({16, -10}, true, false);
    drive_to_position({16, -10}, true);
    // 7
    std::cout << "position: (" << tm->get_current_position().first << ", " << tm->get_current_position().second << "). Heading: " << tm->get_current_heading() << std::endl;

    // drive into  goal
    right_snowplow_out();
    // rotate_to_heading(180);
    rotate_to_position({52, -10}, true);
    left_snowplow_out();
    turbo_drive_distance(50, true, 50.0);
}
