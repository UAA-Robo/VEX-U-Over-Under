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

    // Catapult
    right_catapult_motor.setStopping(vex::brakeType::hold);
    left_catapult_motor.setStopping(vex::brakeType::hold);
    catapult.setStopping(vex::brakeType::hold);
}
