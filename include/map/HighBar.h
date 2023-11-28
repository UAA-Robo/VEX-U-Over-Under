#pragma once
#include <GameElements.h>

class HighBar : private GameElement
{
    /// @brief HighBar Element Constructor
    /// @param id Unique Identifier for Game Element
    /// @param x X Component Position
    /// @param y Y Component Position
    /// @param teamColor Side the highbar is on
    HighBar(
        int id,
        double x,
        double y,
        char teamColor)
        // bool BackTowardsTarget = false)
        // double interactionAngle = 0;)

    {
        this->id = id;
        this->position = std::pair<double, double>(x, y);
        this->teamColor = teamColor;
        //this->isBackAligned = BackTowardsTarget;
        //this->interactionAngle = interactionAngle;
    }
};
