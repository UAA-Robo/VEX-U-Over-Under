#pragma once
#include "vex.h"
#include "Controller.h"

/// @brief     Stores all the electronics (motors, sensors, brain, etc) used by the other classes.
class Hardware
{

public:
    Hardware();
    vex::brain brain;
    Controller controller = Controller(vex::controllerType::primary);

    // Drive train
    vex::motor_group drivetrain = vex::motor_group(
        front_left_drivetrain_motor, middle_left_drivetrain_motor, back_left_drivetrain_motor,
        front_right_drivetrain_motor, middle_right_drivetrain_motor, back_right_drivetrain_motor);

    // Left motors
    vex::motor_group left_drivetrain_motors = vex::motor_group(
        front_left_drivetrain_motor, middle_left_drivetrain_motor, back_left_drivetrain_motor);

    // Right motors
    vex::motor_group right_drivetrain_motors = vex::motor_group(
        front_right_drivetrain_motor, middle_right_drivetrain_motor, back_right_drivetrain_motor);

    // Left
    vex::motor front_left_drivetrain_motor = vex::motor(vex::PORT11, vex::ratio6_1, false);
    vex::motor middle_left_drivetrain_motor = vex::motor(vex::PORT12, vex::ratio6_1, false);
    vex::motor back_left_drivetrain_motor = vex::motor(vex::PORT13, vex::ratio6_1, false);

    // Right
    vex::motor front_right_drivetrain_motor = vex::motor(vex::PORT17, vex::ratio6_1, true);
    vex::motor middle_right_drivetrain_motor = vex::motor(vex::PORT19, vex::ratio6_1, true);
    vex::motor back_right_drivetrain_motor = vex::motor(vex::PORT18, vex::ratio6_1, true);


};