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

    // Intake
    right_intake_motor.setStopping(vex::brakeType::hold);
    left_catapult_motor.setStopping(vex::brakeType::hold);    

    // Catapult
    right_catapult_motor.setStopping(vex::brakeType::brake);
    left_catapult_motor.setStopping(vex::brakeType::brake);
    catapult.setStopping(vex::brakeType::brake);
}
