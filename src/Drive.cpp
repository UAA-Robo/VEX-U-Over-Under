#include "Drive.h"
#include <iostream>

Drive::Drive(Hardware* hardware, RobotConfig* robotConfig, Telemetry* telemetry) {
    hw = hardware;
    rc = robotConfig;
    tm = telemetry;
}
