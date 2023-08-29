#include "Hardware.h"

Hardware::Hardware()
{
    // Individual Motors

    // Left
    front_left_drivetrain_motor.setBrake(vex::brakeType::brake);
    middle_left_drivetrain_motor.setBrake(vex::brakeType::brake);
    back_left_drivetrain_motor.setBrake(vex::brakeType::brake);

    // Right
    front_right_drivetrain_motor.setBrake(vex::brakeType::brake);
    middle_left_drivetrain_motor.setBrake(vex::brakeType::brake);
    back_right_drivetrain_motor.setBrake(vex::brakeType::brake);

    // Motor Groups
    left_drivetrain_motors.setStopping(vex::brakeType::brake);
    right_drivetrain_motors.setStopping(vex::brakeType::brake);

    // Drivetrain
    drivetrain.setStopping(vex::brakeType::brake);
}
