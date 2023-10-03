
#include "Telemetry.h"

Telemetry::Telemetry(Hardware* hardware, RobotConfig* robotConfig)
{
    hw = hardware;
    rc = robotConfig;
    vex::task logTask = vex::task(update_position, this, 1);
}

int Telemetry::update_position(void* param) {
    Telemetry* tm = static_cast<Telemetry*>(param);

        float left_total = tm->hw->left_odometry.position(vex::rotationUnits::rev);
        float right_total = tm->hw->right_odometry.position(vex::rotationUnits::rev);
        float back_total =  tm->hw->back_odometry.position(vex::rotationUnits::rev);

    


    // Updates continuously
    while(true) {

        float left_odometry_revolutions = tm->hw->left_odometry.position(vex::rotationUnits::rev);
        float right_odometry_revolutions = tm->hw->right_odometry.position(vex::rotationUnits::rev);
        float back_odometry_revolutions =  tm->hw->back_odometry.position(vex::rotationUnits::rev);

        left_total += tm->hw->left_odometry.position(vex::rotationUnits::rev);
        right_total += tm->hw->right_odometry.position(vex::rotationUnits::rev);
        back_total +=  tm->hw->back_odometry.position(vex::rotationUnits::rev);

        std :: cout << left_total << " | " << right_total << " | " << back_total << std :: endl;


        float x_displacement = (left_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE
            + right_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE) 
            / (2 * tm->rc->ODOMETRY_LEFT_RIGHT_RADIUS);

        float heading_displacement = (right_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE 
            - left_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE) 
            / (2 * tm->rc->ODOMETRY_LEFT_RIGHT_RADIUS);

        float y_displacement = back_odometry_revolutions  * tm->rc->ODOMETRY_CIRCUMFERENCE 
            + tm->rc->ODOMETRY_BACK_RADIUS * heading_displacement;

        tm->x_position += x_displacement * cos(tm->heading) - y_displacement * sin(tm->heading);
        tm->y_position +=  x_displacement * sin(tm->heading) + y_displacement * cos(tm->heading); 
        tm->heading += (heading_displacement) * M_PI / 180;  // Radians to degrees


        tm->hw->left_odometry.resetPosition();
        tm->hw->right_odometry.resetPosition();
        tm->hw->back_odometry.resetPosition();

        // Print to command line
        std::cout << tm->x_position << "," << tm->y_position << "," << tm->heading << " |"<< left_odometry_revolutions << "|" << right_odometry_revolutions << "|" <<back_odometry_revolutions << std::endl;

        vex::wait(500, vex::timeUnits::msec);
    }
}

double Telemetry ::get_distance_between_points(std::pair<double, double> initial_position, std::pair<double, double> final_position)
{
    double distance_to_final_position = sqrt(pow((final_position.first - initial_position.first), 2) + pow((final_position.second - initial_position.second), 2));
    return distance_to_final_position;
}

double Telemetry ::get_heading_between_points(std::pair<double, double> initial_position, std::pair<double, double> final_position)
{
    double angle_to_final_position = (atan2((final_position.second - initial_position.second), (final_position.first - initial_position.first)) * (180 / (M_PI)));
    return angle_to_final_position;
}

std::pair<double, double> Telemetry::get_current_position()
{
    return this->current_position;
}

double Telemetry::get_current_heading()
{
    return this->current_heading;
}

void Telemetry::set_current_heading(double current_heading)
{
    this->current_heading = current_heading;
}

void Telemetry::set_current_heading(std::pair<double, double> current_position)
{
    this->current_position = current_position;
}