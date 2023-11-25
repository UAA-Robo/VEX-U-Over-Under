#pragma once
#include <GameElements.h>

class Triball : private GameElement
{
    public:
        Triball(
            double x,
            double y,
            //char teamColor,
            //char type,
            bool BackTowardsTarget = false,
            // double minXInterOffset = 0,
            // double maxXInterOffset = 0,
            // double minYInterOffset = 0,
            // double maxYInterOffset = 0
            double interactionAngle)


        {
            this->position = std::pair<double, double>(x, y);
            //this->teamColor = teamColor;
            //this->type = type;
            this->isBackAligned = BackTowardsTarget;
            // this->minXInteractableOffset = minXInterOffset;
            // this->maxXInteractableOffset = maxXInterOffset;
            // this->minYInteractableOffset = minYInterOffset;
            // this->maxYInteractableOffset = maxYInterOffset;
            this->interactionAngle = interactionAngle;
        }
};