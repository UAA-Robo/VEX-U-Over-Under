#pragma once
#include "vex.h"

class Hardware
{

public:
    Hardware();
    vex::brain brain;

    // Drive train
    vex::motor_group::motor_group_motors drive_train = vex::motor_group::motor_group_motors::motor_group_motors(11, 12, 13, 17, 19, 18);

    // radio 20

    // Left wheels: Front 11, Middle 12, Back 13
    vex::motor_group::motor_group_motors left_wheels = vex::motor_group::motor_group_motors::motor_group_motors(11, 12, 13);

    // Right wheels: Front 17, Middle 19, Back 18 (right side reversed?)
    vex::motor_group::motor_group_motors right_wheels = vex::motor_group::motor_group_motors::motor_group_motors(17, 19, 18);

public:
    // Individual Motors

    // Left
    vex::motor front_left_wheel = vex::motor::motor(int32_t 11, vex::gearSetting gears);
    vex::motor middle_left_wheel = vex::motor::motor(int32_t 12, vex::gearSetting gears);
    vex::motor back_left_wheel = vex::motor::motor(int32_t 13, vex::gearSetting gears);

    // Right
    vex::motor front_right_wheel = vex::motor::motor(int32_t 17, vex::gearSetting gears, bool reverse);
    vex::motor middle_right_wheel = vex::motor::motor(int32_t 19, vex::gearSetting gears, bool reverse);
    vex::motor back_right_wheel = vex::motor::motor(int32_t 18, vex::gearSetting gears, bool reverse);
};