#include <utility>

#include "RobotConfig.h"

class Buffer {
    public:
        Buffer(std::pair<double, double> upper_left_corner, std::pair<double, double> bottom_right_corner) {
            this->upper_left_corner = upper_left_corner;
            this->bottom_right_corner = bottom_right_corner;
        }

        /// @brief Checks whether the given position is inside the buffer region.
        /// @param position The coordinates of the given position.
        /// @return True or false whether the position is in the buffer region.
        bool in_buffer(std::pair<double, double> position) {
            if (position.first >= upper_left_corner.first &&
            position.first <= bottom_right_corner.first &&
            position.second <= upper_left_corner.second &&
            position.second >= bottom_right_corner.second) return true;
            else return false;
        }

    private:
        std::pair<double, double> upper_left_corner, bottom_right_corner;
};