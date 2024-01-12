#pragma once
#include "Drive.h"
#include "PathGenerator.h"
#include "vex.h"

/// @brief   Contains the methods for the robot to autonomously navigate.
class AutoDrive : public Drive {

public:
    AutoDrive(Hardware* hardware, RobotConfig* robotConfig, Telemetry* telemetry);

    /// @brief    Main function that initiates to robot.
    void drive();

    /// @brief Determines whether the current game is Skills or Head-To-Head
    bool IS_SKILLS = false;
    

private:
    // bool IS_USING_INERTIA_HEADING = false;
    double robot_angle_offset = 0; // For Granny bc she veers left

    /// @brief Uses drivetrain ENCODERS to rotate to a relativ angle
    /// @param angle Angle in degrees (-360 to 360) where positive angle is counterclockwise rotation
    void rotate_to_relative_angle(double angle);

    /// @brief  Rotates the shortest distance by turning left or right to the heading. Uses odometry sensors.
    /// @param heading  Double that is the counterclockwise angle in degrees from the x asis.
    void rotate_to_heading_odometry(double heading);

    // /// @brief   Rotates the robot to align with the element on the field. Assumes the inertia sensor is set so 0 is at the positive x axis.
    // /// @param gameElement  GameElement that is the object to rotate to.
    // void rotate_to_position(GameElement *gameElement);

    /// @brief  Rotates the robot to align with a coordinate on the field. Assumes the inertia sensor is set so 0 is at the positive x axis.
    /// @param final_position    Pair of doubles {x,y} of the coordinate in inches to align with {0,0} being the center of the field
    /// @param ISBACKROTATION   Boolean that if true, rotates the back of the robot to the coordinate instead of the front
    void rotate_to_position(std::pair<double, double> final_position, bool ISBACKROTATION = false);

    // /// @brief  Rotates the robot to align with a GameElement on the field. Assumes the inertia sensor is set so 0 is at the positive x axis.
    // /// @param gameElement  GameElement that is the object to rotate and drive to.
    // void rotate_and_drive_to_position(GameElement *gameElement);

    /// @brief  Rotates the robot to align with a coordinate on the feild and drives to that position.
    ///         Assumes the inertia sensor is set so 0 is at the positive x axis.
    /// @param position             Pair of doubles {x,y} of the coordinate in inches to align and drive to with {0,0}
    ///                             being the center of the field.
    /// @param ISBACKTOPOSITION     Boolean that if true, rotates the back of the robot to the coordinate instead of the front.
    void rotate_and_drive_to_position(std::pair<double, double> position, bool ISBACKTOPOSITION = false);

    // test
    void rotate_and_drive_to_position(GameElement* element);

    PathGenerator *pg;
    Map *mp;
    Obstacle* zone;
    double drivetrain_offset;
};
