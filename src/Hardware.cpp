#include "Hardware.h"

Hardware::Hardware()
{
    // Individual Motors

    // Left
    front_left_drivetrain_motor.setBrake(vex::brakeType::coast);
    middle_left_drivetrain_motor.setBrake(vex::brakeType::coast);
    back_left_drivetrain_motor.setBrake(vex::brakeType::coast);

    // Right
    front_right_drivetrain_motor.setBrake(vex::brakeType::coast);
    middle_left_drivetrain_motor.setBrake(vex::brakeType::coast);
    back_right_drivetrain_motor.setBrake(vex::brakeType::coast);

    // Motor Groups
    left_drivetrain_motors.setStopping(vex::brakeType::coast);
    right_drivetrain_motors.setStopping(vex::brakeType::coast);

    // Drivetrain
    drivetrain.setStopping(vex::brakeType::coast);
}
