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
            std::cout << "___________" << '\n';
            // mp->in_which_region(source_pos);
            int curr_region = mp->in_which_region(source_pos);
            int target_region = mp->in_which_region(target_pos);
            std::cout << curr_region << " " << target_region << '\n';
            // path.push_back(source_pos);
            // int direction;
            // while (curr_region != target_region) {
            //     if (target_region - curr_region <= 3 &&
            //          (target_region != 1 && curr_region != 5 &&
            //           curr_region != 1 && target_region != 5)
            //         ) direction = 1;
            //     else if (target_region - curr_region > 3 &&
            //          (target_region != 1 && curr_region != 5 &&
            //           curr_region != 1 && target_region != 5)
            //         ) direction = -1;
            //     else if ( target_pos.second > 0 && source_pos.second > 0 &&
            //          ((target_region == 1 && curr_region == 5) ||
            //          (curr_region == 1 && target_region == 5))
            //         ) {
            //             if (target_region == 1 && curr_region == 5) direction = -1;
            //             else direction = 1;
            //         }
            //     else if ( target_pos.second < 0 && source_pos.second < 0 &&
            //          ((target_region == 1 && curr_region == 5) ||
            //          (curr_region == 1 && target_region == 5))
            //         ) {
            //             if (target_region == 1 && curr_region == 5) direction = 1;
            //             else direction = -1;
            //         }
                // path.push_back(mp->get_critical_point(curr_region, true));
                // path.push_back(mp->get_critical_point(curr_region + direction, false));
                // curr_region = mp->in_which_region(
                //     mp->get_critical_point(curr_region + direction, false));
            // }
            // path.push_back(target_pos);

        }