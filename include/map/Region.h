#include <utility>

class Region {

    public:
        Region(int id) : ID(id) {
            std::cout << "Constructor ID" << ID << '\n';
        }

        bool in_region(std::pair<double, double> position) { return true; }

        int ID;

        std::pair<double, double> upper_left_corner, bottom_right_corner,
                                  upper_critical_point, lower_critical_point;
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

        bool in_region(std::pair<double, double> position) {
            std::cout << "ran" << '\n';
            std::cout << (position.first >= upper_left_corner.first) << '\n';
            if (position.first >= upper_left_corner.first &&
            position.first <= bottom_right_corner.first &&
            position.second <= upper_left_corner.second &&
            position.second >= bottom_right_corner.second) {return true;
            std::cout << "id" + ID << '\n';}
            else return false;
        }

        // int ID;
    private:
        

        
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
        
        bool in_region(std::pair<double, double> position) {
            std::cout << position.first << " " << position.second << '\n';
            std::cout << upper_left_corner_1.first << " " << upper_left_corner_1.second << '\n';
            std::cout << bottom_right_corner_1.first << " " << bottom_right_corner_1.second << '\n';
            std::cout << upper_left_corner_2.first << " " << upper_left_corner_2.second << '\n';
            std::cout << bottom_right_corner_2.first << " " << bottom_right_corner_2.second << '\n';
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

        // int ID;
    private:
        std::pair<double, double> upper_left_corner_1, bottom_right_corner_1,
                                  upper_left_corner_2, bottom_right_corner_2;
};