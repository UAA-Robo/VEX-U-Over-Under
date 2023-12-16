#include "AutoDrive.h"


AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry)
    : Drive(hardware, robotConfig, telemetry) {
        mp = new Map(robotConfig);
        pg = new PathGenerator(robotConfig, mp);
    }

void AutoDrive::drive() {}
