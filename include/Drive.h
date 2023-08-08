#pragma once
#include "vex.h"
#include "Hardware.h"
#include "Telemetry.h"
#include "RobotConfig.h"
#include "Logger.h"

class Drive
{
public:
    virtual void drive() {}

protected:
    Drive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry);

    Hardware *hw;
    RobotConfig *rc;
    Telemetry *tm;

    // Needed?
    double output_volt;
    double Kp = 0.9;
    double Ki = 0.3;
    double Kd = 0.6;

    /// @brief      Calculates the velocity in RPMs that the left and right drivetrain wheels should recieve based on
    ///             the horizontal percentage and vertical percentage passed in. Automatically scales the velocities
    ///             for the drivetrain gear inserts.
    /// @param velocity_percent   Pair of doubles {verticalVelocityPercent, horizontalVelocityPercent} from -100 to 100
    ///                     that represent the percentage that the drivetrain should move forward/backward and left/right.
    ///                     For example of verticalVelocityPercent = 50  and horizontalVelocityPercent = 0, the bot
    ///                     will move forward at 50% velocity. Likewise if verticalVelocityPercent = 0  and
    ///                     horizontalVelocityPercent = 50, the drivetrain will rotate to the right at 50% velocity.
    ///                     Any combiniatiion of non-zero verticalVelocityPercents and horizontalVelocityPercents
    ///                     will cause the drivetrain to move in a arc.
    /// @return     Returns a pair of doubles {leftWheelsVelocity, rightWheelsVelocity} that represent the actual velocities in RPM
    ///             (scaled to the gear-ratio) that the wheels on each drivetrain side need to be set to in order move as expected.
    std::pair<double, double> calculate_drivetrain_velocity(std::pair<double, double> velocity_percent);

    /// @brief       Moves the drivetrain based on the horizontal percentage and vertical percentage passed in. Calls calculateDriveTrainVel
    ///             to convert those percentages to actual velocites in RPM.
    /// @param velocity_percent   Pair of doubles {verticalVelocityPercent, horizontalVelocityPercent} from -100 to 100
    ///                     that represent the percentage that the drivetrain should move forward/backward and left/right.
    ///                     For example of verticalVelocityPercent = 50  and horizontalVelocityPercent = 0, the bot
    ///                     will move forward at 50% velocity. Likewise if verticalVelocityPercent = 0  and
    ///                     horizontalVelocityPercent = 50, the drivetrain will rotate to the right at 50% velocity.
    ///                     Any combiniatiion of non-zero verticalVelocityPercents and horizontalVelocityPercents
    ///                     will cause the drivetrain to move in a arc.
    void move_drivetrain(std::pair<double, double> velocity_percent);

    /// @brief      Moves the drivetrain a cetain distance based on the horizontal percentage and vertical percentage passed in.Calls calculateDriveTrainVel
    ///             to convert those percentages to actual velocites in RPM.
    /// @param velocity_percent   Pair of doubles {verticalVelocityPercent, horizontalVelocityPercent} from -100 to 100
    ///                     that represent the percentage that the drivetrain should move forward/backward and left/right.
    ///                     For example of verticalVelocityPercent = 50  and horizontalVelocityPercent = 0, the bot
    ///                     will move forward at 50% velocity. Likewise if verticalVelocityPercent = 0  and
    ///                     horizontalVelocityPercent = 50, the drivetrain will rotate to the right at 50% velocity.
    ///                     Any combiniatiion of non-zero verticalVelocityPercents and horizontalVelocityPercents
    ///                     will cause the drivetrain to move in a arc.
    /// @param distance     Distance in inches to move.
    void move_drivetrain_distance(std::pair<double, double> velocity_percent, double distance);
};
