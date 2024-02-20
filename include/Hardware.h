#pragma once
#include "vex.h"

enum robot_name {
    SCRAT = 0,
    SCRATETTE = 1
};

/// @brief    Stores the electronics (controller, sensors, motors, etc.) used by the other classes.
class Hardware {

public:
    Hardware();
    vex::brain brain;
    vex::controller controller = vex::controller(vex::controllerType::primary);

    // Entire drivetrain for moving forward/backward evenly
    vex::motor_group drivetrain = vex::motor_group(
        front_left_drivetrain_motor, middle_left_drivetrain_motor, back_left_drivetrain_motor,
        front_right_drivetrain_motor, middle_right_drivetrain_motor, back_right_drivetrain_motor);


    // Left motors
    vex::motor_group left_drivetrain_motors = vex::motor_group(
        front_left_drivetrain_motor, middle_left_drivetrain_motor, back_left_drivetrain_motor);

    // Right motors
    vex::motor_group right_drivetrain_motors = vex::motor_group(
        front_right_drivetrain_motor, middle_right_drivetrain_motor, back_right_drivetrain_motor);

    // NOTE: Scrat is 36:1 so DRIVETRAIN_GEAR_RATIO_MULTIPLIER declared 0.5 in hardware
    // Left drivetrain side
    vex::motor front_left_drivetrain_motor = vex::motor(vex::PORT11, vex::ratio18_1, false);
    vex::motor middle_left_drivetrain_motor = vex::motor(vex::PORT12, vex::ratio18_1, false);
    vex::motor back_left_drivetrain_motor = vex::motor(vex::PORT13, vex::ratio18_1, false);
    // Right drivetrain side
    vex::motor front_right_drivetrain_motor = vex::motor(vex::PORT17, vex::ratio18_1, true);
    vex::motor middle_right_drivetrain_motor = vex::motor(vex::PORT19, vex::ratio18_1, true);
    vex::motor back_right_drivetrain_motor = vex::motor(vex::PORT18, vex::ratio18_1, true);

    // Odometry Wheels
    vex::rotation left_odometry =  vex::rotation(vex::PORT6, true);
    vex::rotation right_odometry =  vex::rotation(vex::PORT7);
    vex::rotation back_odometry =  vex::rotation(vex::PORT8);

    // Catapult
    vex::rotation catapult_sensor = vex::rotation(vex::PORT9);

    // Distance Sensor
    vex::distance distanceSensor = vex::distance(vex::PORT20);  // Change port later

    // Pneumatic test
    vex::digital_out right_plow = vex::digital_out(brain.ThreeWirePort.C); //right
    vex::digital_out left_plow = vex::digital_out(brain.ThreeWirePort.B); //left

    // Catapult
    vex::motor left_catapult_motor = vex::motor(vex::PORT15, vex::ratio18_1, true);
    vex::motor right_catapult_motor = vex::motor(vex::PORT16, vex::ratio18_1, false);
    vex::motor_group catapult = vex::motor_group(left_catapult_motor, right_catapult_motor);

    // Intake
    vex::motor left_intake_motor = vex::motor(vex::PORT2, vex::ratio18_1, true);
    vex::motor right_intake_motor = vex::motor(vex::PORT10, vex::ratio18_1, false);
    vex::motor_group intake = vex::motor_group(left_intake_motor, right_intake_motor);

    vex::motor left_intake_expansion_motor = vex::motor(vex::PORT5, vex::ratio18_1, false);
    vex::motor right_intake_expansion_motor = vex::motor(vex::PORT4, vex::ratio18_1, true);
    vex::motor_group intake_expansion = vex::motor_group(left_intake_expansion_motor, right_intake_expansion_motor);

    // Limit switches
    vex::digital_in catapult_limit_switch = vex::digital_in(brain.ThreeWirePort.H);
    
    // Climb motors
    vex::motor front_climb_motor = vex::motor(vex::PORT9, vex::ratio36_1, false);
    vex::motor middle_climb_motor = vex::motor(vex::PORT1, vex::ratio36_1, true);
    vex::motor back_climb_motor = vex::motor(vex::PORT21, vex::ratio36_1, false);
    vex::motor_group climb_motors = vex::motor_group(front_climb_motor, middle_climb_motor, back_climb_motor);

};