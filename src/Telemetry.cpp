
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

        

        const float ODOMETRY_WIDTH = 2 * tm->rc->ODOMETRY_LEFT_RIGHT_RADIUS;

        float left_displacement = left_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE;

        float right_displacement = right_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE;

        float back_displacement = back_odometry_revolutions * tm->rc->ODOMETRY_CIRCUMFERENCE; 
        
        float heading_displacement = (left_displacement - right_displacement) / ODOMETRY_WIDTH;  

        float center_displacement  = (left_displacement + right_displacement) / 2;

        float horizontal_displacement = back_displacement - phi *  tm->rc->ODOMETRY_BACK_RADIUS; 

        float current_heading = tm->heading * M_PI/180;  

        //Easy Math
        //float x_displacement = center_displacement * cos(current_heading) - horizontal_displacement * -sin(current_heading);
        //float y_displacement = center_displacement  * sin(current_heading) + horizontal_displacement * cos(current_heading);

        //Advanced math This assumes constant curviture
        float x_displacement = (center_displacement *
            (cos(current_heading) * sin(heading_displacement)
            + -sin(current_heading) * (1 - cos(heading_displacement)))

            + (horizontal_displacement * 
            (cos(current_heading) * (cos(heading_displacement) -1)) 
            + -sin(current_heading) * sin(heading_displacement)) )/ heading_displacement;
           
        float y_displacement = (center_displacement *
            (sin(current_heading) * sin(heading_displacement)
            + cos(current_heading) * (1 - cos(heading_displacement)))

            + (horizontal_displacement * 
            (sin(current_heading) * (cos(heading_displacement) -1)) 
            +cos(current_heading) * sin(heading_displacement)) )/ heading_displacement;
       
       
        tm->x_position += x_displacement;
        tm->y_position +=  y_displacement;
        tm->heading += ((heading_displacement)  % 2 * M_PI) / (M_PI/ 180);  // Radians to degrees


        tm->hw->left_odometry.resetPosition();
        tm->hw->right_odometry.resetPosition();
        tm->hw->back_odometry.resetPosition();

        // Print to command line
        std::cout << tm->x_position << "," << tm->y_position << "," << tm->heading << std::endl;

        vex::wait(5, vex::timeUnits::msec);
    }
}