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
    bool isBackAligned = false;
    double maxXInteractableOffset;
    double minXInteractableOffset;
    double maxYInteractableOffset;
    double minYInteractableOffset;

    /// @brief Game Element Constructor. This class is designed to have child classes built off of it. Allowing for easy customization and handling of map elements.
    /// @param id ID of game element. This way game elements can be uniquely identified.
    /// @param BackTowardsTarget Used to decide if the back of the robot should be facing the game element
    /// @param minXInterOffset Minimum X Interactable Offset, used to decide if robot can approach element at an angle, or if it needs to be straight on
    /// @param maxXInterOffset Maximum X Interactable Offset, used to decide if robot can approach element at an angle, or if it needs to be straight on
    /// @param minYInterOffset Minimum Y Interactable Offset, used to decide if robot can approach element at an angle, or if it needs to be straight on
    /// @param maxYInterOffset Maximum Y Interactable Offset, used to decide if robot can approach element at an angle, or if it needs to be straight on

    GameElement(
        int id,
        bool BackTowardsTarget = false,
        double minXInterOffset = 0,
        double maxXInterOffset = 0,
        double minYInterOffset = 0,
        double maxYInterOffset = 0)
    {
        this->id = id;
        this->isBackAligned = BackTowardsTarget;
        this->minXInteractableOffset = minXInterOffset;
        this->maxXInteractableOffset = maxXInterOffset;
        this->minYInteractableOffset = minYInterOffset;
        this->maxYInteractableOffset = maxYInterOffset;
    }

public:
    // Setters

    void setIsBackAligned(bool isBackAligned) { this->isBackAligned = isBackAligned; }

    void setXInteractableOffset(dbpair XInterOffset)
    {
        this->minXInteractableOffset = XInterOffset.first;
        this->maxXInteractableOffset = XInterOffset.second;
    }
    void setYInteractableOffset(dbpair YInterOffset)
    {
        this->minYInteractableOffset = YInterOffset.first;
        this->maxYInteractableOffset = YInterOffset.second;
    }
    void setXYMinInteractableOffset(dbpair XYInterOffset)
    {
        this->minXInteractableOffset = XYInterOffset.first;
        this->minYInteractableOffset = XYInterOffset.second;
    }
    void setXYMaxInteractableOffset(dbpair XYInterOffset)
    {
        this->maxXInteractableOffset = XYInterOffset.first;
        this->maxYInteractableOffset = XYInterOffset.second;
    }

    void SetMinXInteractableOffset(double minInterOffset) { this->minXInteractableOffset = minInterOffset; }
    void SetMaxXInteractableOffset(double maxInterOffset) { this->maxXInteractableOffset = maxInterOffset; }
    void SetMinYInteractableOffset(double minInterOffset) { this->minYInteractableOffset = minInterOffset; }
    void SetMaxYInteractableOffset(double maxInterOffset) { this->maxYInteractableOffset = maxInterOffset; }

    // Getters
    int GetID() { return id; }
    bool GetAlignment() { return isBackAligned; }

    dbpair GetXInteractableOffset() { return dbpair(minXInteractableOffset, maxXInteractableOffset); };
    dbpair GetYInteractableOffset() { return dbpair(minYInteractableOffset, maxYInteractableOffset); };
    dbpair GetXYMinInteractableOffset() { return dbpair(minXInteractableOffset, minYInteractableOffset); };
    dbpair GetXYMaxInteractableOffset() { return dbpair(maxXInteractableOffset, maxYInteractableOffset); };

    double GetMinXInteractableOffset() { return minXInteractableOffset; }
    double GetMaxXInteractableOffset() { return maxXInteractableOffset; }
    double GetMinYInteractableOffset() { return minYInteractableOffset; }
    double GetMaxYInteractableOffset() { return maxYInteractableOffset; }
};