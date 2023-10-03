#pragma once
#include "vex.h"
#include "Hardware.h"
#include "Telemetry.h"
#include "RobotConfig.h"
#include "Logger.h"

/// @brief   Parent class for methods and variable shared between UserDrive and AutoDrive.
class Drive
{
public:
    virtual void drive() {}

protected:
    Drive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry);

    Hardware *hw;
    RobotConfig *rc;
    Telemetry *tm;

    /// @brief    Calculates the velocity in RPMs that the left and right drivetrain wheels should
    ///           receive based on the horizontal percentage and vertical percentage passed in.
    ///           Automatically scales the velocities for the drivetrain gear inserts.
    /// @param velocity_percent    Pair of doubles from -100% to 100%,
    ///                            {verticalVelocityPercent, horizontalVelocityPercent}.
    ///    For example of verticalVelocityPercent = 50  and horizontalVelocityPercent = 0, the bot
    ///    will move forward at 50% velocity. If verticalVelocityPercent = 0  and
    ///    horizontalVelocityPercent = 50, the drivetrain will rotate to the right at 50% velocity.
    ///    Any combination of non-zero verticalVelocityPercents and horizontalVelocityPercents
    ///    will cause the drivetrain to move in a arc.
    /// @return     Returns a pair of doubles {leftWheelsVelocity, rightWheelsVelocity} that
    ///             represents the actual velocities in RPM (scaled to the gear-ratio) that the
    ///             wheels on each drivetrain side need.
    std::pair<double, double> calculate_drivetrain_velocity(std::pair<double, double> velocity_percent);

    /// @brief       Moves the drivetrain based on the horizontal percentage and vertical percentage
    ///              passed in.
    /// @param velocity_percent   Pair of doubles from -100 to 100:
    ///                           {verticalVelocityPercent, horizontalVelocityPercent}
    void move_drivetrain(std::pair<double, double> velocity_percent);

    /// @brief      Moves the drivetrain a certain distance based on the horizontal percentage and
    ///             vertical percentage passed in.
    /// @param velocity_percent   Pair of doubles from -100 to 100:
    ///                           {verticalVelocityPercent, horizontalVelocityPercent}
    void move_drivetrain_distance(std::pair<double, double> velocity_percent, double distance);

    // /// @brief       Moves the drivetrain based on the horizontal percentage and vertical percentage passed in. Calls calculateDriveTrainVel
    // ///             to convert those percentages to actual velocites in RPM.
    // /// @param velocity_percent   Pair of doubles {verticalVelocityPercent, horizontalVelocityPercent} from -100 to 100
    // ///                     that represent the percentage that the drivetrain should move forward/backward and left/right.
    // ///                     For example of verticalVelocityPercent = 50  and horizontalVelocityPercent = 0, the bot
    // ///                     will move forward at 50% velocity. Likewise if verticalVelocityPercent = 0  and
    // ///                     horizontalVelocityPercent = 50, the drivetrain will rotate to the right at 50% velocity.
    // ///                     Any combiniatiion of non-zero verticalVelocityPercents and horizontalVelocityPercents
    // ///                     will cause the drivetrain to move in a arc.
    // void moveDriveTrain(std::pair<double, double> velocity_percent);

    // /// @brief      Moves the drivetrain a cetain distance based on the horizontal percentage and vertical percentage passed in. Calls calculateDriveTrainVel
    // ///             to convert those percentages to actual velocites in RPM.
    // /// @param velocity_percent   Pair of doubles {verticalVelocityPercent, horizontalVelocityPercent} from -100 to 100
    // ///                     that represent the percentage that the drivetrain should move forward/backward and left/right.
    // ///                     For example of verticalVelocityPercent = 50  and horizontalVelocityPercent = 0, the bot
    // ///                     will move forward at 50% velocity. Likewise if verticalVelocityPercent = 0  and
    // ///                     horizontalVelocityPercent = 50, the drivetrain will rotate to the right at 50% velocity.
    // ///                     Any combiniatiion of non-zero verticalVelocityPercents and horizontalVelocityPercents
    // ///                     will cause the drivetrain to move in a arc.
    // /// @param distance     Distance in inches to move.
    // void moveDriveTrainDistance(std::pair<double, double> velocity_percent, double distance);

    // /// @brief      Calculates the velocity in RPMs that the left and right drivetrain wheels should recieve based on
    // ///             the horizontal percentage and vertical percentage passed in. Automatically scales the velocities
    // ///             for the drivetrain gear inserts.
    // /// @param velocity_percent   Pair of doubles {verticalVelocityPercent, horizontalVelocityPercent} from -100 to 100
    // ///                     that represent the percentage that the drivetrain should move forward/backward and left/right.
    // ///                     For example of verticalVelocityPercent = 50  and horizontalVelocityPercent = 0, the bot
    // ///                     will move forward at 50% velocity. Likewise if verticalVelocityPercent = 0  and
    // ///                     horizontalVelocityPercent = 50, the drivetrain will rotate to the right at 50% velocity.
    // ///                     Any combiniatiion of non-zero verticalVelocityPercents and horizontalVelocityPercents
    // ///                     will cause the drivetrain to move in a arc.
    // /// @return     Returns a pair of doubles {leftWheelsVelocity, rightWheelsVelocity} that represent the actual velocities in RPM
    // ///             (scaled to the gear-ratio) that the wheels on each drivetrain side need to be set to in order move as expected.
    // std::pair<double, double> calculateDriveTrainVel(std::pair<double, double> velocity_percent); //{verticalvelocity_percent, horizontalvelocity_percent}
};
