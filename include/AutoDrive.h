#pragma once
#include "Drive.h"
#include "PathGenerator.h"
#include "vex.h"

/// @brief   Contains the methods for the robot to autonomously navigate.
class AutoDrive : public Drive {

public:
    AutoDrive(Hardware* hardware, RobotConfig* robotConfig, Telemetry* telemetry);

    /// @brief Main function that initiates to robot.
    void drive();

    /// @brief Determines whether the current game is Skills or Head-To-Head
    bool IS_SKILLS = false;

    /// @brief Offset
    double drivetrain_offset;
    

private:
    PathGenerator *pg;
    Map *mp;
    RobotConfig *rc;

    std::vector<std::pair<double, double>> path;

    /// @brief Starts the auto strategy for SKILLS.
    void execute_skills_plan();

    /// @brief Starts the odometry testing.
    void test_odometry();

    /// @brief Starts turbo drive testing.
    void test_turbo();

    double robot_angle_offset = 0; // For Granny bc she veers left

    /// @brief Drives to all coordinates stored in the local path vector.
    void drive_along_path();

    /// @brief Pathfinds to a target position and drives to it.
    /// @param target_position The position to drive to.
    void pathfind_and_drive_to_position(std::pair<double, double> target_position);

    /// @brief Drive a specified distance at high speed.
    /// @param distance The distance to drive.
    /// @param ISBACKTOPOSITION True if driving backwards, false otherwise.
    void turbo_drive_distance(double distance, bool ISBACKTOPOSITION);

    /// @brief Uses drivetrain ENCODERS to rotate to a relativ angle
    /// @param angle Angle in degrees (-360 to 360) where positive angle is counterclockwise rotation
    void rotate_to_relative_angle(double angle);

    /// @brief Rotates the shortest distance by turning left or right to the heading. Uses odometry sensors.
    /// @param heading Double that is the counterclockwise angle in degrees from the x asis.
    void rotate_to_heading(double heading);

    /// @brief Rotates the robot to align with a coordinate on the field.  Assumes 0 degrees is at 
    /// the positive x axis.
    /// @param final_position Pair of doubles {x,y} of the coordinate in inches to align with 
    /// {0,0} being the center of the field
    /// @param ISBACKROTATION Boolean that if true, rotates the back of the robot to the 
    /// coordinate instead of the front
    void rotate_to_position(std::pair<double, double> final_position, bool ISBACKROTATION = false);

    /// @brief   Rotates the robot to align with the element on the field. Assumes the inertia 
    ///     sensor is set so 0 is at the positive x axis.
    /// @param element  InteractionObject that is the object to rotate and drive to.
    /// @param IS_BACK_POSITION Rotates so the back is aligned toward the object.
    /// @param IS_OFFSET If true, rotates so that the element is aligned with the front (or back)
    ///      of the bot instead of at the center.
    /// @param IS_OFFSET_EXTRA If true, rotates so that the element is aligned with the front 
    ///     (or back) of the bot instead of at the center with extra room to turn.
    void rotate_to_position(InteractionObject *element, bool IS_BACK_POSITION = false, 
        bool IS_OFFSET = false, bool IS_OFFSET_EXTRA = false);


    /// @brief Rotates the robot to align with a coordinate on the feild and drives to that 
    /// position. Assumes 0 degrees is at the positive x axis.
    /// @param position Pair of doubles {x,y} of the coordinate in inches to align and drive to, 
    /// with {0,0} being the center of the field.
    /// @param ISBACKTOPOSITION Boolean that if true, rotates the back of the robot to the 
    /// coordinate instead of the front.
    void rotate_and_drive_to_position(std::pair<double, double> position, 
        bool ISBACKTOPOSITION = false);

    /// @brief  Rotates the robot to align with a InteractionObject on the field. Assumes the 
    /// inertia sensor is set so 0 is at the positive x axis.
    /// @param element  InteractionObject that is the object to rotate and drive to.
    /// @param IS_BACK_POSITION Drives backward toward the object.
    /// @param IS_OFFSET If true, drives so that the element is at the front (or back) of the bot
    ///     instead of at the center.
    /// @param IS_OFFSET_EXTRA If true, drives so that the element is at the front (or back) of the bot
    ///     instead of at the center with extra room to turn.
    void rotate_and_drive_to_position(InteractionObject *element, bool IS_BACK_POSITION= false, 
        bool IS_OFFSET=false, bool IS_OFFSET_EXTRA=false);


    /// @brief Moves the drivetrain STRAIGHT until the distance between the current position add
    ///  (goal) position is almost 0.
    /// @param position Pair of doubles: {X, Y}
    /// @param ISBACKTOPOSITION A bool value that determines the direction the bot moves.
    void drive_to_position(std::pair<double, double> position, bool ISBACKTOPOSITION = false);

    /// @brief  Rotates the shortest distance by turning left or right to the heading. Assumes the inertia sensor is set so 0 is at the positive x axis.
    /// @param heading  Double that is the counterclockwise angle in degrees from the x asis.
    void turbo_spin(double heading);

    /// @brief Executes the plow strategy in the game.
    void run_plow_strategy();
};
