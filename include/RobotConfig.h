#pragma once
#include "Hardware.h"

class RobotConfig {
public:
    RobotConfig(Hardware* hw);

private:
    Hardware* hw;

};
