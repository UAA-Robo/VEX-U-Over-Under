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
    
    /// @brief Gets bot ready to start by retracting intake and catapult
    //void setup_bot();

protected:
    Drive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry);

    Hardware *hw;
    RobotConfig *rc;
    Telemetry *tm;
    // PID *pid;

    /// @brief Calculates the velocity in RPMs that the left and right drivetrain wheels should
    /// receive based on the horizontal percentage and vertical percentage passed in.
    /// Automatically scales the velocities for the drivetrain gear inserts.
    /// @param velocity_percent Pair of doubles from -100% to 100%, 
    /// {verticalVelocityPercent, horizontalVelocityPercent}.For example if 
    /// verticalVelocityPercent = 50  and horizontalVelocityPercent = 0, the bot will move forward
    /// at 50% velocity. If verticalVelocityPercent = 0 and horizontalVelocityPercent = 50,
    /// the drivetrain will rotate to the right at 50% velocity. Any combination of non-zero
    /// verticalVelocityPercents and horizontalVelocityPercents  will cause the drivetrain to move 
    /// in a arc.
    /// @return Returns a pair of doubles {leftWheelsVelocity, rightWheelsVelocity} that
    /// represents the actual velocities in RPM (scaled to the gear-ratio) that the
    /// wheels on each drivetrain side need.
    std::pair<double, double> calculate_drivetrain_velocity(std::pair<double, double> velocity_percent);

    /// @brief Moves the drivetrain based on the horizontal percentage and vertical percentage
    /// passed in.
    /// @param velocity_percent Pair of doubles from -100 to 100:
    ///  {verticalVelocityPercent, horizontalVelocityPercent}
    void move_drivetrain(std::pair<double, double> velocity_percent);

    /// @brief      Moves the drivetrain a certain distance based on the horizontal percentage and 
    ///             vertical percentage passed in.
    /// @param velocity_percent   Pair of doubles from -100 to 100:
    ///                           {verticalVelocityPercent, horizontalVelocityPercent} 
    void move_drivetrain_distance(std::pair<double, double> velocity_percent, double distance);
    
    /// @brief Activates the intake
    void activate_intake();

    /// @brief Stops the intake
    void stop_intake();

    /// @brief Expands the intake
    void expand_intake();

    /// @brief Retracts the intake
    void retract_intake();

    /// @brief stops the intake
    void stop_intake_expansion();

    /// @brief Pushes air out when X button is pressed to expand snowplow.
    void snowplow_out();

    /// @brief Pushes air into right_snowplow independently
    void right_snowplow_out();

    /// @brief Pushes air into left_snowplow independently
    void left_snowplow_out();

    /// @brief Sucks air in when Y button is pressed to retract snowplow.
    void snowplow_in();

    /// @brief Sucks air into the right_plow independently
    void right_snowplow_in();

    /// @brief Sucks air into the left_plow independently
    void left_snowplow_in();

    /// @brief Spins the catapult motors until limit switch is hit or launch/stop_catapult is 
    ///     called. Loops forever because it is on it's own thread.
    /// @param param For passing the telemetry class (this)
    /// @return Does not return anything but VEX requires type int return.
    static int run_catapult_thread(void* param);

    /// @brief Trigger the catapult motors to start turning to launch
    void start_catapult();

    /// @brief Trigger the catapult to stop and reset at the limit switch.
    void stop_catapult();


    void run_catapult_catapult_strategy(int number_triballs=10);

    void turbo_drive_distance(double distance, bool IS_REVERSE, double velocity = 80);



    bool START_CATAPULT_LAUNCH = false;
    bool CATAPULT_STOPPED = true;
    bool SNOWPLOW_OUT = false;
};
