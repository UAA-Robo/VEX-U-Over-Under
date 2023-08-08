#pragma once
#include "vex.h"

class Hardware
{

public:
    Hardware();
    vex::brain brain;
    vex::controller controller = vex::controller(vex::primary);

    // radio 20

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

private:
    /*
    Individual Motors

    @brief Creates a new motor object on the port specified Sets the reversed flag and the gear setting for the new motor object.
    @param index The port index for this motor. The index is zero-based.
    @param gears Sets the gear's setting for the new motor object.
    @param reverse Sets the reverse flag for the new motor object.
    */

    // Left
    vex::motor front_left_drivetrain_motor = vex::motor::motor(vex::PORT11, vex::ratio18_1, false);
    vex::motor middle_left_drivetrain_motor = vex::motor::motor(vex::PORT12, vex::ratio18_1, false);
    vex::motor back_left_drivetrain_motor = vex::motor::motor(vex::PORT13, vex::ratio18_1, false);

    // Right
    vex::motor front_right_drivetrain_motor = vex::motor::motor(vex::PORT17, vex::ratio18_1, true);
    vex::motor middle_right_drivetrain_motor = vex::motor::motor(vex::PORT19, vex::ratio18_1, true);
    vex::motor back_right_drivetrain_motor = vex::motor::motor(vex::PORT18, vex::ratio18_1, true);
};