#pragma once
#include "vex.h"
#include "Hardware.h"
#include "RobotConfig.h"

/// @brief    Contains all the methods for calculation the robots position.
class Telemetry {
public:
    Telemetry(Hardware* hardware, RobotConfig* robotConfig);

    static int update_position(void* param);

    float x_position = 0;
    float y_position = 0;
    
    // In Degrees
    float heading = 0;
private:
    Hardware* hw;
    RobotConfig* rc;    


};



