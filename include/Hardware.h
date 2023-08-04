#pragma once
#include "vex.h"

class Hardware
{

public:
    Hardware();
    vex::brain brain;

    // Drive train
    vex::motor_group drive_train = vex::motor_group(11, 12, 13, 17, 19, 18);

    // radio 20

    // Left wheels: Front 11, Middle 12, Back 13
    vex::motor_group left_wheels = vex::motor_group(11, 12, 13);

    // Right wheels: Front 17, Middle 19, Back 18 (right side reversed?)
    vex::motor_group right_wheels = vex::motor_group(17, 19, 18);

public:
    // Individual Motors

    // Left
    vex::motor front_left_wheel = motor(int32_t 11);
    vex::motor middle_left_wheel = motor(int32_t 12);
    vex::motor back_left_wheel = motor(int32_t 13);

    // Right
    vex::motor front_right_wheel = vex::motor(int32_t 17, bool reverse);
    vex::motor middle_right_wheel = vex::motor(int32_t 19, bool reverse);
    vex::motor back_right_wheel = vex::motor(int32_t 18, bool reverse);
};