
#include "Telemetry.h"



Telemetry::Telemetry(Hardware* hardware, RobotConfig* robotConfig)
{
    hw = hardware;
    rc = robotConfig;
    vex::task logTask = vex::task(update_position, this, 1);
}

int Telemetry::update_position(void* param) {
    Telemetry* tm = static_cast<Telemetry*>(param);

    // Updates continuously
    while(true) {

        double left_odometry_revolutions = tm->hw->left_odometry.position(vex::rotationUnits::rev);
        double right_odometry_revolutions = tm->hw->right_odometry.position(vex::rotationUnits::rev);
        double back_odometry_revolutions =  tm->hw->back_odometry.position(vex::rotationUnits::rev);

        tm->hw->left_odometry.resetPosition();
        tm->hw->right_odometry.resetPosition();
        tm->hw->back_odometry.resetPosition();


        const double ODOMETRY_WIDTH = 2 * tm->rc->ODOMETRY_LEFT_RIGHT_RADIUS;
        
        double left_displacement = left_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE;
        double right_displacement = right_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE;
        double back_displacement = back_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE; 
        double heading_displacement = (left_displacement - right_displacement) / ODOMETRY_WIDTH;  
        double center_displacement  = (left_displacement + right_displacement) / 2;


        // can use either sin or angle, sin uses straight line, angle uses arc
        double horizontal_displacement = back_displacement - heading_displacement *  tm->rc->ODOMETRY_BACK_RADIUS; 

        double current_heading = std::fmod(tm->odometry_heading + heading_displacement, 2 * M_PI);  

        //Easy Math
        double x_displacement = center_displacement * cos(current_heading) - horizontal_displacement * -sin(current_heading);
        double y_displacement = center_displacement  * sin(current_heading) + horizontal_displacement * cos(current_heading);


        tm->odometry_x_position += x_displacement;
        tm->odometry_y_position += y_displacement;
    

        //convert deg for modding
        tm->odometry_heading += heading_displacement;
        tm->odometry_heading = std::fmod(tm->odometry_heading * (180/ M_PI), 360);

        // Print to command line
        std::cout << tm->odometry_x_position << "," << tm->odometry_y_position << "," << tm->odometry_heading << " |"<< left_odometry_revolutions << "|" << right_odometry_revolutions << "|" <<back_odometry_revolutions << std::endl; //already in deg

        //convert heading back into rads for calculations
        tm->odometry_heading = tm->odometry_heading * (M_PI / 180.0);

        // Rotation sensor is updated every 20ms so 30ms wait should be fine 
        // https://www.vexforum.com/t/rotation-sensor-update-rate-optical-shaft-encoder-vs-rotation-sensor/106917
        vex::wait(30, vex::timeUnits::msec); 
    }
}

double Telemetry::get_distance_between_points(std::pair<double, double> initial_position, std::pair<double, double> final_position)
{
    double distance_to_final_position = sqrt(pow((final_position.first - initial_position.first), 2) + pow((final_position.second - initial_position.second), 2));
    return distance_to_final_position;

    std::cout << "Distance() called successful!" << std::endl;  // TDOD: remove later
}

double Telemetry ::get_heading_between_points(std::pair<double, double> initial_position, std::pair<double, double> final_position)
{
    double angle_to_final_position = (atan2((final_position.second - initial_position.second), (final_position.first - initial_position.first)) * (180 / (M_PI)));
    return angle_to_final_position;
}

std::pair<double, double> Telemetry::get_current_position() // Revised for odometry
{
    return std::make_pair(this->odometry_x_position, this->odometry_y_position);
}

double Telemetry::get_current_heading() // Revised for odometry
{
    return this->odometry_heading; 
}


// May use later (if GPS)

// void Telemetry::set_current_heading(double current_heading)
// {
//     this->current_heading = current_heading;
// }

// void Telemetry::set_current_heading(std::pair<double, double> current_position)
// {
//     this->current_position = current_position;
//         vex::wait(50, vex::timeUnits::msec);
// }
