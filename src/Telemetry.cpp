
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

        float left_odometry_revolutions = tm->hw->left_odometry.angle(vex::rotationUnits::rev);
        float right_odometry_revolutions = tm->hw->right_odometry.angle(vex::rotationUnits::rev);
        float back_odometry_revolutions =  tm->hw->back_odometry.angle(vex::rotationUnits::rev);


        float x_displacement = (left_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE
            + right_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE) 
            / (2 * tm->rc->ODOMETRY_LEFT_RIGHT_RADIUS);

        float heading_displacement = (left_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE 
            - right_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE) 
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
        std::cout << tm->x_position << "," << tm->y_position << "," << tm->heading << std::endl;

        vex::wait(5, vex::timeUnits::msec);
    }
}