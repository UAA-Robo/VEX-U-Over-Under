#include "AutoDrive.h"


AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) {
        mp = new Map(telemetry, robotConfig, isSkills);
        pg = new PathGenerator(robotConfig, mp);
        double drivetrain_offset = 2 * robotConfig->DRIVETRAIN_LENGTH;

        // set starting zone
        for (int i = 22; i < 26; i++)
        {
            Obstacle* newZone =  static_cast<Obstacle*>(mp->mapElements[i]);
            if (newZone->inZone(tm->odometry_position))
            {
                zone = newZone;
                break;
            }
        }
    }

void AutoDrive::drive() {
    // Set braking
    hw->left_drivetrain_motors.setStopping(vex::brakeType::brake);
    hw ->right_drivetrain_motors.setStopping(vex::brakeType::brake);
    move_drivetrain_distance_odometry({-24, 0}, true);


    std::pair<double, double> current_pos = {58.0, -58.0};
    std::pair<double, double> new_target_pos = {-25.0, 25.0};
    std::vector<std::pair<double, double>> path;
    pg->generate_path(path, current_pos, new_target_pos);
    for (int i = 0; i < path.size(); ++i) {
        std::cout << path[i].first << " " << path[i].second << '\n';
    }
}

void AutoDrive::rotate_to_heading_odometry(double heading)
{
    // Corrects heading to be from 0-360 from the x axis counterclockwise if applicable
    heading = fmod(heading, 360);
    if (heading < 0) heading += 360;

    double curr_heading = tm->get_current_heading();

    double min_velocity = 10;
    double max_velocity = 50;
    double stopping_aggression = 0.03; // higher number is higher aggression (steeper slope)
    double velocity;
    double initial_heading = curr_heading;
    
    // 1 if counterclockwise, -1 if clockwise depending on which direction is shorter to turn
    double turn_direction = (fabs(heading - tm->get_current_heading()) <= 180) ? 1: -1;


    // Turn wheels opposite of each other
    hw->left_drivetrain_motors.spin(vex::directionType::fwd, -min_velocity * turn_direction, vex::velocityUnits::pct);
    hw->right_drivetrain_motors.spin(vex::directionType::fwd, min_velocity * turn_direction, vex::velocityUnits::pct);

    // Turn until within 2 degrees of desired heading
    while (fabs(heading - tm->get_current_heading()) > 1 ) {
        curr_heading = tm->get_current_heading();

        // Speeds up as leaving initial angle and slows down as approaching destination
        if (curr_heading <= (initial_heading + heading)/2) {
            // First half of distance
            velocity = atan(fabs(curr_heading - initial_heading)) * 2 * (max_velocity-min_velocity) / M_PI + min_velocity;
            //std::cout << "Here1: " << velocity << std::endl;
        } else {
            // Second half of distance
            velocity = atan(stopping_aggression * fabs(heading - curr_heading)) * 2 * max_velocity / M_PI;
            //std::cout << "Here2: " << velocity << std::endl;
        }

        hw->left_drivetrain_motors.setVelocity(-velocity * turn_direction, vex::velocityUnits::pct);
        hw->right_drivetrain_motors.setVelocity(velocity * turn_direction, vex::velocityUnits::pct);
        
        vex::wait(35, vex::timeUnits::msec);
        std::cout << "Drive: " << curr_heading << ", " << velocity << "," << tm->odometry_x_position << "," << tm->odometry_y_position << "," << tm->odometry_heading << std::endl;
    }

    hw->drivetrain.stop();                       // TODO: Give a vel value later
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

void AutoDrive::rotate_and_drive_to_position(GameElement* element)
{
    InteractionObject* object = static_cast<InteractionObject*>(element); 
    std::pair<double, double> position = object->GetPosition();

    if (*(object->GetInteractionAngle()) == tm->get_current_heading()) // if can interact head on
    {
        rotate_to_position(position, object->GetAlignment());
        move_drivetrain_distance_odometry(position, object->GetAlignment());
    }
    else
    {
        double interactionAngle = *(object->GetInteractionAngle());
        std::pair<double, double> original_position = position;
        if (interactionAngle == 0) { position.second -= drivetrain_offset; }
        else if (interactionAngle == 45) { position.first += 1.4 * drivetrain_offset; position.second -= 1.4 * drivetrain_offset; }
        else if (interactionAngle == 90) { position.first += drivetrain_offset; }
        else if (interactionAngle == 135) { position.first += 1.4 * drivetrain_offset; position.second -= 1.4 * drivetrain_offset; }
        else if (interactionAngle == 180) { position.second += drivetrain_offset; }
        else if (interactionAngle == 270) { position.first -= drivetrain_offset; }
        else if (interactionAngle == -45) { position.first -= 1.4 * drivetrain_offset; position.second -= 1.4 * drivetrain_offset; }
        else if (interactionAngle == -135) { position.first -= 1.4 * drivetrain_offset; position.second += 1.4 * drivetrain_offset; }

        rotate_to_position(original_position, object->GetAlignment());
        move_drivetrain_distance_odometry(original_position, object->GetAlignment());
    }
}
