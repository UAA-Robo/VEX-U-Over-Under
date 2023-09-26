
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

double Telemetry ::getDistanceBtwnPoints(std::pair<double, double> initPos, std::pair<double, double> finalPos)
{
    double distanceToFinalPosition = sqrt(pow((finalPos.first - initPos.first), 2) + pow((finalPos.second - initPos.second), 2));
    return distanceToFinalPosition;
}

double Telemetry ::getHeadingBtwnPoints(std::pair<double, double> initPos, std::pair<double, double> finalPos)
{
    double angleToFinalPosition = (atan2((finalPos.second - initPos.second), (finalPos.first - initPos.first)) * (180 / (M_PI)));
    return angleToFinalPosition;
}

std::pair<double, double> Telemetry::getCurrPosition()
{
    return this->currentPosition;
}

void Telemetry::setCurrPosition(std::pair<double, double> currPos)
{
    this->currentPosition = currPos;
}

double Telemetry::getCurrHeading()
{
    return this->currHeading;
}

void Telemetry::setCurrHeading(double currHeading)
{
    this->currHeading = currHeading;
}

void Telemetry::headingErrorCorrection(double errorBounds)
{
    const int numChecks = 3;
    double gpsEstimate = getGPSHeading();
    int x = 0;
    while (fabs(gpsEstimate - currHeading) > errorBounds && x < numChecks)
    {
        gpsEstimate = getGPSHeading();
        ++x;
    }

    if (x == numChecks)
    {
        hw->controller.Screen.setCursor(3, 1);
        hw->controller.Screen.print("Used Encoder Heading");
        return;
    }
    hw->controller.Screen.setCursor(3, 1);
    hw->controller.Screen.print("Used GPS Heading");
    this->currHeading = gpsEstimate;
}