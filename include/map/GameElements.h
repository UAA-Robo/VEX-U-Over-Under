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
private:
    int id;
    std::pair<double, double> position;
    char teamColor;
    char type;
    double maxXInteractableOffset;
    double minXInteractableOffset;
    double maxYInteractableOffset;
    double minYInteractableOffset;
    bool isBackAligned = false;
    double interactionAngle;

protected:
    /// @brief Game Element Constructor. This class is designed to have child classes built off of it. Allowing for easy customization and handling of map elements.
    /// @param id ID of game element. This way game elements can be uniquely identified.
    /// @param x X Component of the position
    /// @param y Y component of the position
    /// @param rotation Rotation of game element
    /// @param teamColor If an element is part of a specific team then this is set to that team's identifier
    /// @param type Used to differentiate elements by type if they are all in one list
    /// @param BackTowardsTarget Used to decide if the back of the robot should be facing the game element
    /// @param minXInterOffset Minimum X Interactable Offset, used to decide if robot can approach element at an angle, or if it needs to be straight on
    /// @param maxXInterOffset Maximum X Interactable Offset, used to decide if robot can approach element at an angle, or if it needs to be straight on
    /// @param minYInterOffset Minimum Y Interactable Offset, used to decide if robot can approach element at an angle, or if it needs to be straight on
    /// @param maxYInterOffset Maximum Y Interactable Offset, used to decide if robot can approach element at an angle, or if it needs to be straight on
    /// @param The angle required for the bot to interact with the Game Element.
    GameElement(
        int id,
        double x,
        double y,
        double rotation,
        char teamColor,
        char type,
        bool BackTowardsTarget = false,
        double minXInterOffset = 0,
        double maxXInterOffset = 0,
        double minYInterOffset = 0,
        double maxYInterOffset = 0
        double interactionAngle = 0;)
    {
        this->id = id;
        this->position = std::pair<double, double>(x, y);
        this->rotation = rotation;
        this->teamColor = teamColor;
        this->type = type;
        this->isBackAligned = BackTowardsTarget;
        this->minXInteractableOffset = minXInterOffset;
        this->maxXInteractableOffset = maxXInterOffset;
        this->minYInteractableOffset = minYInterOffset;
        this->maxYInteractableOffset = maxYInterOffset;
        this->interactionAngle = interactionAngle;
    }

public:
    // Setters

    void SetPosition(double x, double y) { this->position = std::pair<double, double>(x, y); }
    void SetPosition(std::pair<double, double> position) { this->position = position; }
    void SetRotation(double rotation) { this->rotation = rotation; }
    void SetTeamColor(char color) { this->teamColor = color; }
    void SetType(char type) { this->type = type; }
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

    std::pair<double, double> GetPosition() { return position; }
    std::pair<double, double> GetPositionWithMinOffset()
    {
        return {position.first + minXInteractableOffset, position.second + minYInteractableOffset};
    }
    double GetXPosition() { return position.first; }
    double GetYPosition() { return position.second; }
    int GetID() { return id; }
    double GetRotation() { return rotation; }
    char GetTeamColor() { return teamColor; }
    char GetType() { return type; }
    bool GetAlignment() { return isBackAligned; }

    dbpair GetXInteractableOffset() { return dbpair(minXInteractableOffset, maxXInteractableOffset); };
    dbpair GetYInteractableOffset() { return dbpair(minYInteractableOffset, maxYInteractableOffset); };
    dbpair GetXYMinInteractableOffset() { return dbpair(minXInteractableOffset, minYInteractableOffset); };
    dbpair GetXYMaxInteractableOffset() { return dbpair(maxXInteractableOffset, maxYInteractableOffset); };

    double GetMinXInteractableOffset() { return minXInteractableOffset; }
    double GetMaxXInteractableOffset() { return maxXInteractableOffset; }
    double GetMinYInteractableOffset() { return minYInteractableOffset; }
    double GetMaxYInteractableOffset() { return maxYInteractableOffset; }

    dbpair test() { return position + position; }
};