#pragma once
#include "Drive.h"

class UserDrive : public Drive{
public:
    UserDrive(Hardware* hardware, RobotConfig* robotConfig, Telemetry* telemetry);
    void drive();

};