#pragma once
#include <utility>

class Region {

    public:
        Region(int id) : ID(id) {
            // std::cout << "Constructor ID" << ID << '\n';
        }

        /// @brief Checks whether the given position is inside the region.
        /// @param position The coordinates of the given position.
        /// @return True or false whether the position is inside the region.
        virtual bool in_region(std::pair<double, double> position) { return true; }

        int ID;

        std::pair<double, double> upper_left_corner, bottom_right_corner,
                                  upper_critical_point, lower_critical_point;
        /// @brief Checks whether the given position is a critical point of the region.
        /// @param position The coordinates of the given position.
        /// @return True or false whether the position is a critical point of the region.
        bool is_critical_point(std::pair<double, double> position) {
            if (position == upper_critical_point ||
                position == lower_critical_point) return true;
            else return false;
        }
};

class SimpleRegion : public Region {
    public:
        SimpleRegion(
            int region_id,
            std::pair<double, double> upper_left_corner,
            std::pair<double, double> bottom_right_corner,
            std::pair<double, double> upper_critical_point,
            std::pair<double, double> lower_critical_point
            ) : Region(region_id) {
                    this->upper_left_corner = upper_left_corner;
                    this->bottom_right_corner = bottom_right_corner;
                    this->upper_critical_point = upper_critical_point;
                    this->lower_critical_point = lower_critical_point;
                }

        /// @brief Checks whether the given position is inside the region.
        /// @param position The coordinates of the given position.
        /// @return True or false whether the position is inside the region.
        bool in_region(std::pair<double, double> position) override {
            if (position.first >= upper_left_corner.first &&
            position.first <= bottom_right_corner.first &&
            position.second <= upper_left_corner.second &&
            position.second >= bottom_right_corner.second) return true;
            else return false;
        }       
};

class CompositeRegion : public Region {

    public:
        CompositeRegion(
            int region_id,
            std::pair<double, double> upper_left_corner_1,
            std::pair<double, double> bottom_right_corner_1,
            std::pair<double, double> upper_left_corner_2,
            std::pair<double, double> bottom_right_corner_2,
            std::pair<double, double> upper_critical_point,
            std::pair<double, double> lower_critical_point
        ) : Region(region_id) {
                this->upper_left_corner_1 = upper_left_corner_1;
                this->bottom_right_corner_1 = bottom_right_corner_1;
                this->upper_left_corner_2 = upper_left_corner_2;
                this->bottom_right_corner_2 = bottom_right_corner_2;
                this->upper_critical_point = upper_critical_point;
                this->lower_critical_point = lower_critical_point;
            }
        
        /// @brief Checks whether the given position is inside the region.
        /// @param position The coordinates of the given position.
        /// @return True or false whether the position is inside the region.
        bool in_region(std::pair<double, double> position) override {
            if (
                (
                    position.first >= upper_left_corner_1.first &&
                    position.first <= bottom_right_corner_1.first &&
                    position.second <= upper_left_corner_1.second &&
                    position.second >= bottom_right_corner_1.second
                ) ||
                (
                    position.first >= upper_left_corner_2.first &&
                    position.first <= bottom_right_corner_2.first &&
                    position.second <= upper_left_corner_2.second &&
                    position.second >= bottom_right_corner_2.second
                )
            ) return true;
            else return false;
        }

    private:
        std::pair<double, double> upper_left_corner_1, bottom_right_corner_1,
                                  upper_left_corner_2, bottom_right_corner_2;
};