#pragma once
#include "vex.h"
#include "Hardware.h"
#include "RobotConfig.h"

/// @brief Contains all the methods for calculation the robots position.
class Telemetry {
public:
    Telemetry(Hardware *hardware, RobotConfig *robotConfig);

    /// @brief Updates the odometry position and heading continuously using the odometry wheels.
    /// @param param For passing the telemetry class (this)
    /// @return Does not return anything but VEX requires type int return.
    static int update_position(void *param);

    //In inches
    /// @brief Calculates the distance (in inches) between to points
    /// @param initial_position Pair of doubles {x, y} that represent the first coordinate 
    /// (where the origin is the center of the field)
    /// @param final_position Pair of doubles {x, y} that represent the second coordinate 
    /// (where the origin is the center of the field)
    /// @return Returns the distance in inches
    double get_distance_between_points(std::pair<double, double> initial_position, 
        std::pair<double, double> final_position);

    /// @brief Calculates the counterclockwise angle from the x axis (of the field) 
    /// to the line formed from by 2 points.
    /// @param initial_position Pair of doubles {x, y} that represent the first coordinate 
    /// (where the origin is the center of the field)
    /// @param final_position Pair of doubles {x, y} that represent the second coordinate 
    /// (where the origin is the center of the field)
    /// @return Returns angle in degrees.
    double get_heading_between_points(std::pair<double, double> initial_position, 
        std::pair<double, double> final_position);

    /// @brief Gets the current odometry position
    /// @return     Returns {odometry_x_position, odometry_y_position} in inches
    std::pair<double, double> get_current_position();

    /// @brief Gets the current odometry heading
    /// @return Returns heading in degree
    double get_current_heading();

    /// @brief Sets the current heading that odometry updates
    void set_heading(double heading);


    /// @brief Sets the current position that odometry updates
    void set_position(std::pair<double, double> position);
    
    /// public so map can access address
    double odometry_heading;  //In degrees

private:
    Hardware *hw;
    RobotConfig *rc;

    double odometry_x_position;  // In inches
    double odometry_y_position;  // In inches

    double previous_left_odometry, previous_right_odometry, previous_back_odometry;
};
