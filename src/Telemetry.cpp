
#include "Telemetry.h"



Telemetry::Telemetry(Hardware* hardware, RobotConfig* robotConfig) {
    hw = hardware;
    rc = robotConfig;
    // odometry_x_position = rc->starting_pos.first;
    // odometry_y_position = rc->starting_pos.second;
    // odometry_heading = rc->starting_heading;

    if (rc->ROBOT == SCRAT) vex::task logTask = vex::task(update_position, this, 1);
}

int Telemetry::update_position(void* param) {
    Telemetry* tm = static_cast<Telemetry*>(param);

    for (int i = 0; i < 100; ++i) {
        tm->hw->left_odometry.setPosition(0.0, vex::rotationUnits::rev);
        tm->hw->right_odometry.setPosition(0.0, vex::rotationUnits::rev);
        tm->hw->back_odometry.setPosition(0.0, vex::rotationUnits::rev);
    }
    vex::wait(100, vex::timeUnits::msec); 

    // Updates continuously
    while(true) {



        double left_odometry_revolutions = tm->hw->left_odometry.position(vex::rotationUnits::rev);
        double right_odometry_revolutions = tm->hw->right_odometry.position(vex::rotationUnits::rev);
        double back_odometry_revolutions =  tm->hw->back_odometry.position(vex::rotationUnits::rev);

        tm->hw->left_odometry.resetPosition();
        tm->hw->right_odometry.resetPosition();
        tm->hw->back_odometry.resetPosition();

        const double ODOMETRY_WIDTH = 2 * tm->rc->ODOMETRY_LEFT_RIGHT_RADIUS;
        
        // Relative to robot
        double left_displacement = left_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE;
        double right_displacement = right_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE;
        double back_displacement = back_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE; 
        double heading_displacement = (right_displacement - left_displacement) / ODOMETRY_WIDTH;  
        double vertical_displacement  = (left_displacement + right_displacement) / 2;


        // can use either sin or angle, sin uses straight line, angle uses arc
        double horizontal_displacement = back_displacement + heading_displacement *  tm->rc->ODOMETRY_BACK_RADIUS; 

        if (tm->rc->ROBOT == SCRATETTE) {

            double turn_displacement  = tm->rc->ODOMETRY_BACK_OFFSET / cos(heading_displacement)
                + (tm->rc->ODOMETRY_BACK_RADIUS  
                - tm->rc->ODOMETRY_BACK_OFFSET * tan(heading_displacement)) 
                * sin(heading_displacement);

            
            horizontal_displacement = back_displacement + tm->rc->ODOMETRY_BACK_OFFSET 
                - turn_displacement;

        }
        
        //convert heading into rads for calculations
        double current_heading = std::fmod(tm->odometry_heading * (M_PI / 180.0) + heading_displacement, 2 * M_PI);  // Correct

        double x_displacement = vertical_displacement * cos(current_heading) - horizontal_displacement * sin(current_heading);
        double y_displacement = vertical_displacement  * sin(current_heading) + horizontal_displacement * cos(current_heading);

        // Convert heading back to degrees for storage and make sure it's from 0-360
        double current_heading_degrees = std::fmod(current_heading * (180/ M_PI), 360);
        if (current_heading_degrees < 0) current_heading_degrees += 360;

        tm -> odometry_heading  = current_heading_degrees;
        tm->odometry_x_position += x_displacement;
        tm->odometry_y_position += y_displacement;

        // Print to command line
        // std::cout << tm->odometry_x_position << "," << tm->odometry_y_position << "," 
        // << tm->odometry_heading << " |"<< left_odometry_revolutions << "|"
        //  << right_odometry_revolutions << "|" << back_odometry_revolutions << std::endl; 

        // Rotation sensor is updated every 20ms so 30ms wait should be fine 
        // https://www.vexforum.com/t/rotation-sensor-update-rate-optical-shaft-encoder-vs-rotation-sensor/106917
        vex::wait(30, vex::timeUnits::msec); 
    }
}

double Telemetry::get_distance_between_points(std::pair<double, double> initial_position, 
    std::pair<double, double> final_position) {
    double distance_to_final_position = sqrt(pow((final_position.first - initial_position.first), 2) 
    + pow((final_position.second - initial_position.second), 2));
    return distance_to_final_position;
}

double Telemetry ::get_heading_between_points(std::pair<double, double> initial_position, 
    std::pair<double, double> final_position) {
    double angle_to_final_position = (atan2((final_position.second - initial_position.second), 
        (final_position.first - initial_position.first)) * (180 / (M_PI)));
    return angle_to_final_position;
}

std::pair<double, double> Telemetry::get_current_position() {
    return std::make_pair(this->odometry_x_position, this->odometry_y_position);
}

double Telemetry::get_current_heading() {
    return this->odometry_heading; 
}


void Telemetry::set_heading(double heading) {
    this->odometry_heading = heading; 
}

void Telemetry::set_position(std::pair<double, double> position) {
    this->odometry_x_position = position.first; 
    this->odometry_y_position = position.second; 
}