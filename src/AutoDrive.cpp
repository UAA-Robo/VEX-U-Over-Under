#include "AutoDrive.h"


AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry)
    : Drive(hardware, robotConfig, telemetry) {
        mp = new Map(robotConfig);
        pg = new PathGenerator(robotConfig, mp);
    }

void AutoDrive::drive() {

    std::pair<double, double> current_pos = {-20.1, 0.00};
    std::pair<double, double> new_target_pos = {20.05, 52.34};
    std::vector<std::pair<double, double>> path;
    pg->generate_path(path, current_pos, new_target_pos);
    // for (int i = 0; i < path.size(); ++i) {
    //     std::cout << path[i].first << " " << path[i].second << '\n';
    // }
}