#include <vector>
#include <utility>

#include "map/Map.h"
#include "RobotConfig.h"

class PathGenerator {

    public:
        PathGenerator(RobotConfig* robotConfig, Map* map);

        void generate_path(
            std::vector<std::pair<double, double>> &path,
            std::pair<double, double> source_pos,
            std::pair<double, double> target_pos
        );
    
    private:
        RobotConfig *rc;
        Map *mp;
};