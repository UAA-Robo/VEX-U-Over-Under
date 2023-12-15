#pragma once
#include <GameElements.h>

class InteractionObject : private GameElement
{
    public:
        /// @brief Triball Element Constructor
        /// @param id Unique Identifier for Game Element
        /// @param x X Component Position
        /// @param y Y Component Position
        /// @param teamColor Side of the field the triball is on
        InteractionObject(
            int id,
            double x,
            double y,
            char teamColor,
            double interactionAngle)

        {
            this->id = id;
            this->position = std::pair<double, double>(x, y);
            this->teamColor = teamColor;
            this->interactionAngle = interactionAngle;
            this->isBackAligned = false;
        }
};