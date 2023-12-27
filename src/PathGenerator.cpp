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
            int curr_region = mp->in_which_region(source_pos, -1, 1);
            int target_region = mp->in_which_region(target_pos, -1, 1);
            std::cout << curr_region << " " << target_region << '\n';
            path.push_back(source_pos);
            int direction;
            // while (curr_region != target_region && path[path.size() - 1] != target_pos) {
            // for (int i = 0; i < 4; ++i) {
                if (target_region - curr_region <= 3 &&
                     (target_region != 1 && curr_region != 5 &&
                      curr_region != 1 && target_region != 5)
                    ) direction = 1;
                else if (target_region - curr_region > 3 &&
                     (target_region != 1 && curr_region != 5 &&
                      curr_region != 1 && target_region != 5)
                    ) direction = -1;
                else if ( target_pos.second > 0 && source_pos.second > 0 &&
                     ((target_region == 1 && curr_region == 5) ||
                     (curr_region == 1 && target_region == 5))
                    ) {
                        if (target_region == 1 && curr_region == 5) direction = -1;
                        else direction = 1;
                    }
                else if ( target_pos.second < 0 && source_pos.second < 0 &&
                     ((target_region == 1 && curr_region == 5) ||
                     (curr_region == 1 && target_region == 5))
                    ) {
                        if (target_region == 1 && curr_region == 5) direction = 1;
                        else direction = -1;
                    }
                else {
                    if (target_region - curr_region <= 3) direction = 1;
                    else direction = -1;
                }
            while (curr_region != target_region) {
                int new_region = curr_region + direction;
                if (new_region == -1) new_region = 7;
                else if (new_region == 8) new_region = 0;
                // std::cout << curr_region << " NR" << new_region << '\n';
                if (path[path.size() - 1] != mp->get_critical_point(curr_region, (direction == 1) ? true : false))
                    path.push_back(mp->get_critical_point(curr_region, (direction == 1) ? true : false));
                if (path[path.size() - 1] != mp->get_critical_point(new_region, (direction == 1) ? false : true))
                    path.push_back(mp->get_critical_point(new_region, (direction == 1) ? false : true));
                curr_region = mp->in_which_region(
                    mp->get_critical_point(new_region, (direction == 1) ? false : true), curr_region, direction);
                // std::cout << curr_region << '\n';
                // for (int i = 0; i < path.size(); ++i)
                // std::cout << path[i].first << " " << path[i].second << '\n';
            }
            path.push_back(target_pos);

        }