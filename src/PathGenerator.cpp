#include <iostream>

#include "PathGenerator.h"

PathGenerator::PathGenerator(RobotConfig* robotConfig, Map* map) {
    rc = robotConfig;
    mp = map;
}

void PathGenerator::generate_path(
            std::vector<std::pair<double, double>> &path,
            std::pair<double, double> source_pos,
            std::pair<double, double> target_pos
        ) {
            int curr_region = mp->in_which_region(source_pos);
            int target_region = mp->in_which_region(target_pos);
            while (curr_region != target_region) {

            }

        }