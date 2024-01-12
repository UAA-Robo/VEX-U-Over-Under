#pragma once
#include <utility>

/*
    dbpair is a commonly used pair so it's defined here with some operators to make using it easier
*/

typedef std::pair<double, double> dbpair;

inline dbpair operator+(const dbpair &x, const dbpair &y)
{
    return dbpair(x.first + y.first, x.second + y.second);
}
inline dbpair operator-(const dbpair &x, const dbpair &y)
{
    return dbpair(x.first - y.first, x.second - y.second);
}

/*
    The GameElement class is designed to be a building block for specific game elements.
    This allows for the simplification of game element classes and easier handling of game elements in general.
*/

class GameElement
{
protected:
    int id;
    bool IS_BACK_ALIGNED = false;

    /// @brief Game Element Constructor. This class is designed to have child classes built off of it. Allowing for easy customization and handling of map elements.
    /// @param id ID of game element. This way game elements can be uniquely identified.
    /// @param BackTowardsTarget Used to decide if the back of the robot should be facing the game element
    GameElement(
        int id,
        bool BackTowardsTarget = false)
    {
        this->id = id;
        this->IS_BACK_ALIGNED = BackTowardsTarget;
    }

    public:
        bool get_alignment() { return IS_BACK_ALIGNED; }
};