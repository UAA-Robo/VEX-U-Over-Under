#include <utility>

#include "RobotConfig.h"

class Buffer {
    public:
        Buffer(std::pair<int, int> upper_left_corner, std::pair<int, int> bottom_right_corner) :
        upper_left_corner(upper_left_corner), bottom_right_corner(bottom_right_corner) {

        }

        bool in_buffer(std::pair<int, int> position) {
            if (position.first >= upper_left_corner.first - DRIVETRAINWIDTH &&
            position.first <= bottom_right_corner.first &&
            position.second <= upper_left_corner.second &&
            position.second >= bottom_right_corner.second) return true;
            else return false;
        }

    private:
        std::pair<int, int> upper_left_corner, bottom_right_corner;
}