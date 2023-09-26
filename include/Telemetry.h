#pragma once
#include "vex.h"
#include "Hardware.h"
#include "RobotConfig.h"

/// @brief    Contains all the methods for calculation the robots position.
class Telemetry
{
private:
    std::pair<double, double> currentPosition;
    double currHeading;

public:
    Telemetry(Hardware *hardware, RobotConfig *robotConfig);

    static int update_position(void *param);
    double getDistanceBtwnPoints(std::pair<double, double> initPos, std::pair<double, double> finalPos);
    double getHeadingBtwnPoints(std::pair<double, double> initPos, std::pair<double, double> finalPos);
    float x_position = 0;
    float y_position = 0;
    float heading = 0;
    void headingErrorCorrection(double errorBounds);

private:
    Hardware *hw;
    RobotConfig *rc;
};
