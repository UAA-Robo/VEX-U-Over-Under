#pragma once
#include <GameElements.h>

class LoadZone : private GameElement
{
    public:
    /// @brief LoadZone constructor
    /// @param id Unique Identifier for Game Element
    /// @param x X component position
    /// @param y Y component position
    /// @param currentCapacity Current number of triballs in loading zone
    /// @param teamColor Side of the field the loading zone is on
    /// @param interactionAngle The bot must interact at this angle
    LoadZone(
        int id,
        double x,
        double y,
        int currentCapacity,
        char teamColor,
        double interactionAngle)

    {
        this->id = id;
        this->position = std::pair<double, double>(x, y);
        this->currentCapacity = currentCapacity;
        this->teamColor = teamColor;
        this->interactionAngle = interactionAngle;
        this->isBackAligned = false;
    }

    private:
        int currentCapacity;

};