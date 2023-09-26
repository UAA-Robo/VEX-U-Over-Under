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

    /// @brief              Calculates the distance (in inches) between to points
    /// @param initPos      Pair of doubles {x, y} that represent the first coordinate (where the origin is the center of the field)
    /// @param finalPos     Pair of doubles {x, y} that represent the second coordinate (where the origin is the center of the field)
    /// @return             Returns the distance in inches
    double getDistanceBtwnPoints(std::pair<double, double> initPos, std::pair<double, double> finalPos);

    /// @brief              Calculates the counterclockwise angle from the x axis (of the field) to the line formed from by 2 points.
    /// @param initPos      Pair of doubles {x, y} that represent the first coordinate (where the origin is the center of the field)
    /// @param finalPos     Pair of doubles {x, y} that represent the second coordinate (where the origin is the center of the field)
    /// @return             Returns angle in degrees.
    double getHeadingBtwnPoints(std::pair<double, double> initPos, std::pair<double, double> finalPos);
    float x_position = 0;
    float y_position = 0;
    float heading = 0;

    /// @brief      Gets the current position of the robot
    /// @return     Returns the currentPosition
    std::pair<double, double> getCurrPosition();

    void setCurrHeading(double currPos);
    void headingErrorCorrection(double errorBounds = 5);
    double getCurrHeading();

    /// @brief      Checks the current position and compares it with the gps position. If it is within a certain constraint the gps is used
    //              Not 100% accureate at the moment
    void positionErrorCorrection(double errorBounds = 5);

private:
    Hardware *hw;
    RobotConfig *rc;
};
