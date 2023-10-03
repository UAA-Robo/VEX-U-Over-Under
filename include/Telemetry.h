#pragma once
#include "vex.h"
#include "Hardware.h"
#include "RobotConfig.h"

/// @brief    Contains all the methods for calculation the robots position.
class Telemetry
{
private:
    std::pair<double, double> current_position;
    double current_heading;

public:
    Telemetry(Hardware *hardware, RobotConfig *robotConfig);

    static int update_position(void *param);

    /// @brief              Calculates the distance (in inches) between to points
    /// @param initial_position      Pair of doubles {x, y} that represent the first coordinate (where the origin is the center of the field)
    /// @param final_position     Pair of doubles {x, y} that represent the second coordinate (where the origin is the center of the field)
    /// @return             Returns the distance in inches
    double get_distance_between_points(std::pair<double, double> initial_position, std::pair<double, double> final_position);

    /// @brief              Calculates the counterclockwise angle from the x axis (of the field) to the line formed from by 2 points.
    /// @param initial_position      Pair of doubles {x, y} that represent the first coordinate (where the origin is the center of the field)
    /// @param final_position     Pair of doubles {x, y} that represent the second coordinate (where the origin is the center of the field)
    /// @return             Returns angle in degrees.
    double get_heading_between_points(std::pair<double, double> initial_position, std::pair<double, double> final_position);
    float x_position = 0;
    float y_position = 0;
    float heading = 0;

    /// @brief      Gets the current position of the robot
    /// @return     Returns the current_position
    std::pair<double, double> get_current_position();

    /// @brief      Sets the curent position of the robot
    /// @param current_position
    void set_current_heading(std::pair<double, double> current_position);

    void set_current_heading(double current_position);
    double get_current_heading();

private:
    Hardware *hw;
    RobotConfig *rc;
};
