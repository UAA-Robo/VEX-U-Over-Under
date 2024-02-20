#include <iostream>

#include "PathGenerator.h"

PathGenerator::PathGenerator(RobotConfig* robotConfig, Map* map, Telemetry* telemetry) {
    rc = robotConfig;
    mp = map;
    tm = telemetry;
}

std::vector<std::pair<double, double>> PathGenerator::generate_path(
            std::pair<double, double> source_pos,
            std::pair<double, double> target_pos
        ) {
            std::vector<std::pair<double, double>> path;
            int curr_region = mp->in_which_region(source_pos, -1, 1);
            int target_region = mp->in_which_region(target_pos, -1, 1);
            std::cout << curr_region << " " << target_region << '\n';
            path.push_back(source_pos);
            int direction = 0;
            if (curr_region == target_region) {
                path.push_back(target_pos);
                return path;
            }
            // while (curr_region != target_region && path[path.size() - 1] != target_pos) {
            // for (int i = 0; i < 4; ++i) {
            if (((target_region - curr_region <= 3 && target_region - curr_region > 0) ||
                (target_region - curr_region <= -5)) &&
                    ((target_region != 1 || curr_region != 5) &&
                    (curr_region != 1 || target_region != 5))
                ) direction = 1;
            else if (((target_region - curr_region > 3 && target_region - curr_region < 5) ||
                    (target_region - curr_region < 0 && target_region - curr_region > -5) ||
                    (target_region - curr_region) >= 5)  &&
                    ((target_region != 1 || curr_region != 5) &&
                    (curr_region != 1 || target_region != 5))
                ) direction = -1;
            else if (target_pos.second > 0.0 && source_pos.second > 0.0 &&
                    ((target_region == 1 && curr_region == 5) ||
                    (curr_region == 1 && target_region == 5))
                ) {
                    if (target_region == 1 && curr_region == 5) direction = -1;
                    else direction = 1;
                }
            else if (target_pos.second < 0.0 && source_pos.second < 0.0 &&
                    ((target_region == 1 && curr_region == 5) ||
                    (curr_region == 1 && target_region == 5))
                ) {
                    if (target_region == 1 && curr_region == 5) direction = 1;
                    else direction = -1;
                }
            else if ((target_pos.second == 0.0 || source_pos.second == 0.0) &&
                    ((target_region == 1 && curr_region == 5) ||
                    (curr_region == 1 && target_region == 5))
                ) {
                    if (target_region == 1 && curr_region == 5) {
                        if (target_pos.second > 0.0 || source_pos.second > 0.0) direction = -1;
                        if (target_pos.second < 0.0 || source_pos.second < 0.0) direction = 1;
                    }
                    else {
                        if (target_pos.second > 0.0 || source_pos.second > 0.0) direction = 1;
                        if (target_pos.second < 0.0 || source_pos.second < 0.0) direction = -1;
                    }
                }
            
            else {
                if (target_region - curr_region <= 3) direction = 1;
                else direction = -1;
            }
            while (path[path.size() - 1] != target_pos) {
                int new_region = curr_region + direction;
                if (new_region == -1) new_region = 7;
                else if (new_region == 8) new_region = 0;
                if (path[path.size() - 1] != mp->get_critical_point(curr_region, (direction == 1) ? true : false))
                    path.push_back(mp->get_critical_point(curr_region, (direction == 1) ? true : false));
                if (path[path.size() - 1] != mp->get_critical_point(new_region, (direction == 1) ? false : true))
                    path.push_back(mp->get_critical_point(new_region, (direction == 1) ? false : true));
                curr_region = mp->in_which_region(
                    mp->get_critical_point(new_region, (direction == 1) ? false : true), curr_region, direction);
                if (curr_region == target_region) path.push_back(target_pos);
            }
            std::vector<std::pair<double, double>> efficient_path = {source_pos};

            // for (int i = 1; i < path.size() - 2; ++i) {
            //     if (!path_is_clear(path.at(i-1), path.at(i+1))) {
            //         efficient_path.push_back(path.at(i));
            //     }
            // }
            // efficient_path.push_back(target_pos);
            // for (int i = 0; i < efficient_path.size(); ++i) {
            //     std::cout << "(" << efficient_path.at(i).first << ", " << efficient_path.at(i).second << ")" << '\n';
            // }
            // return efficient_path;
            return path;
        }

bool PathGenerator::path_is_clear(
    std::pair<double, double> source_pos,
    std::pair<double, double> target_pos
) {
    // std::pair<double, double> curr_pos = source_pos;
    double heading_between_points = atan((target_pos.second - source_pos.second) / 
                                            (target_pos.first - source_pos.first));
    for (double i = 0.0; i < tm->get_distance_between_points(source_pos, target_pos); i += 0.5) {
        // std::cout << i << '\n';
        std::pair<double, double> curr_pos = source_pos;
        curr_pos.first += cos(heading_between_points) * i;
        curr_pos.second += sin(heading_between_points) * i;
        // if (int(i) % 2 == 0) std::cout << "(" << curr_pos.first << ", " << curr_pos.second << ")\n";
        if (mp->in_buffer(curr_pos)) return false;
    }
    return true;
}