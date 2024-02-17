#include "Hardware.h"

Hardware::Hardware()
{
    // Motor breaking

    // Drivetrain
    front_left_drivetrain_motor.setBrake(vex::brakeType::coast);
    middle_left_drivetrain_motor.setBrake(vex::brakeType::coast);
    back_left_drivetrain_motor.setBrake(vex::brakeType::coast);

    front_right_drivetrain_motor.setBrake(vex::brakeType::coast);
    middle_left_drivetrain_motor.setBrake(vex::brakeType::coast);
    back_right_drivetrain_motor.setBrake(vex::brakeType::coast);

    
    left_drivetrain_motors.setStopping(vex::brakeType::coast);
    right_drivetrain_motors.setStopping(vex::brakeType::coast);

    drivetrain.setStopping(vex::brakeType::coast);

    // Set timeout
    left_drivetrain_motors.setTimeout(2, vex::timeUnits::sec);
    right_drivetrain_motors.setTimeout(2, vex::timeUnits::sec);
    drivetrain.setTimeout(2, vex::timeUnits::sec);

    // Intake
    right_intake_motor.setStopping(vex::brakeType::hold);
    right_intake_motor.setStopping(vex::brakeType::hold);  

    right_intake_expansion_motor.setStopping(vex::brakeType::coast);
    left_intake_expansion_motor.setStopping(vex::brakeType::coast);
    intake_expansion.setStopping(vex::brakeType::coast);


    left_odometry.resetPosition();
    right_odometry.resetPosition();
    back_odometry.resetPosition();

    // Catapult
    catapult.setStopping(vex::brakeType::brake);
    catapult.setStopping(vex::brakeType::brake);

    catapult_sensor.resetPosition();

    // Retract plows initially
    right_plow.set(true);
    left_plow.set(true);

    // Odometry
    left_odometry.resetPosition();
    right_odometry.resetPosition();
    back_odometry.resetPosition();

    // Distance sensor
    distanceSensor.installed();
}
