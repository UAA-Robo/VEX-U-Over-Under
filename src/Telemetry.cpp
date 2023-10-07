
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

        float left_odometry_revolutions = tm->hw->left_odometry.position(vex::rotationUnits::rev);
        float right_odometry_revolutions = tm->hw->right_odometry.position(vex::rotationUnits::rev);
        float back_odometry_revolutions =  tm->hw->back_odometry.position(vex::rotationUnits::rev);

        

        const float ODOMETRY_WIDTH = 2 * tm->rc->ODOMETRY_LEFT_RIGHT_RADIUS;

        float left_displacement = left_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE;

        float right_displacement = right_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE;

        float back_displacement = back_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE; 
        
        float heading_displacement = (left_displacement - right_displacement) / ODOMETRY_WIDTH;  

        float center_displacement  = (left_displacement + right_displacement) / 2;


        // can use either sin or angle, sin uses straight line, angle uses arc
        float horizontal_displacement = back_displacement - heading_displacement *  tm->rc->ODOMETRY_BACK_RADIUS; 

        float current_heading = std::fmod(tm->heading + heading_displacement, 2 * M_PI);  

        //Easy Math
        float x_displacement = center_displacement * cos(current_heading) - horizontal_displacement * -sin(current_heading);
        float y_displacement = center_displacement  * sin(current_heading) + horizontal_displacement * cos(current_heading);
       
        tm->x_position += x_displacement;
        tm->y_position +=  y_displacement;
    

        //tm->heading = std::fmod(tm->heading, 2 * M_PI);
        //tm->heading_displacement = std::fmod(tm->heading_displacement, 2 * M_PI);
        
        //convert deg for modding
        tm->heading += heading_displacement;
        tm->heading = std::fmod(tm->heading * (180/ M_PI), 360);

        tm->hw->left_odometry.resetPosition();
        tm->hw->right_odometry.resetPosition();
        tm->hw->back_odometry.resetPosition();

        // Print to command line
        std::cout << tm->x_position << "," << tm->y_position << "," << tm->heading<< std::endl; //already in deg

        //convert heading back into rads for calculations
        tm->heading = tm->heading * (M_PI / 180.0);

        vex::wait(50, vex::timeUnits::msec);
    }
}