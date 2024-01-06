#include <vector>
#include <utility>

#include "map/Map.h"
#include "RobotConfig.h"

/// @brief Contains the methods and classes needed to map a path between points.
class PathGenerator {

    public:
        PathGenerator(RobotConfig* robotConfig, Map* map);

        /// @brief Populates the path vector with the points to travel from source_pos to target_pos
        /// @param path The path vector to modify.
        /// @param source_pos The current position of the robot.
        /// @param target_pos The target position for the robot.
        void generate_path(
            std::vector<std::pair<double, double>> &path,
            std::pair<double, double> source_pos,
            std::pair<double, double> target_pos
        );
    
    private:
        RobotConfig *rc;
        Map *mp;
};