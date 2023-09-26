#pragma once
#include "Drive.h"
#include "vex.h"

/// @brief   Contains the methods for the robot to autonomously navigate.
class AutoDrive : public Drive {

public:
    AutoDrive(Hardware* hardware, RobotConfig* robotConfig, Telemetry* telemetry);

    /// @brief    Main function that initiates to robot.
    void drive();

private:
    bool IS_USING_INERTIA_HEADING = false;
    double robotAngleOffset = 0; // For Granny bc she veers left

    /// @brief Uses drivetrain ENCODERS to rotate to a relativ angle
    /// @param angle Angle in degrees (-360 to 360) where positive angle is counterclockwise rotation
    void rotateToRelativeAngle(double angle);

    /// @brief  Rotates the shortest distance by turning left or right to the heading. Assumes the inertia sensor is set so 0 is at the positive x axis.
    /// @param heading  Double that is the counterclockwise angle in degrees from the x asis.
    void rotateToHeading(double heading);

    // /// @brief   Rotates the robot to align with the element on the field. Assumes the inertia sensor is set so 0 is at the positive x axis.
    // /// @param gameElement  GameElement that is the object to rotate to.
    // void rotateToPosition(GameElement *gameElement);

    /// @brief  Rotates the robot to align with a coordinate on the field. Assumes the inertia sensor is set so 0 is at the positive x axis.
    /// @param finalPosition    Pair of doubles {x,y} of the coordinate in inches to align with {0,0} being the center of the field
    /// @param ISBACKROTATION   Boolean that if true, rotates the back of the robot to the coordinate instead of the front
    void rotateToPosition(std::pair<double, double> finalPosition, bool ISBACKROTATION = false);

    // /// @brief  Rotates the robot to align with a GameElement on the field. Assumes the inertia sensor is set so 0 is at the positive x axis.
    // /// @param gameElement  GameElement that is the object to rotate and drive to.
    // void rotateAndDriveToPosition(GameElement *gameElement);

    /// @brief  Rotates the robot to align with a coordinate on the feild and drives to that position.
    ///         Assumes the inertia sensor is set so 0 is at the positive x axis.
    /// @param position             Pair of doubles {x,y} of the coordinate in inches to align and drive to with {0,0}
    ///                             being the center of the field.
    /// @param ISBACKTOPOSITION     Boolean that if true, rotates the back of the robot to the coordinate instead of the front.
    void rotateAndDriveToPosition(std::pair<double, double> position, bool ISBACKTOPOSITION = false);
};
