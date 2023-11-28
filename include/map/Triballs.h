#pragma once
#include <GameElements.h>

class Triball : private GameElement
{
    public:
        /// @brief Triball Element Constructor
        /// @param id Unique Identifier for Game Element
        /// @param x X Component Position
        /// @param y Y Component Position
        /// @param teamColor Side of the field the triball is on
        Triball(
            int id;
            double x,
            double y,
            // double minXInterOffset = 0,
            // double maxXInterOffset = 0,
            // double minYInterOffset = 0,
            // double maxYInterOffset = 0
            //double interactionAngle,
            char teamColor)


        {
            this->id = id;
            this->position = std::pair<double, double>(x, y);
            // this->minXInteractableOffset = minXInterOffset;
            // this->maxXInteractableOffset = maxXInterOffset;
            // this->minYInteractableOffset = minYInterOffset;
            // this->maxYInteractableOffset = maxYInterOffset;
            this->interactionAngle = 0;                         // Change to tm->odometry_heading + headingAdjust
            this->teamColor = teamColor;
            this->isBackAligned = false;
        }
};