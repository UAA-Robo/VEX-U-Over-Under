#pragma once
#include <GameElements.h>

class LoadZone : private GameElement
{
    public:
    LoadZone(
        //int id,
        double x,
        double y,
        int currentCapacity,
        char teamColor,
        // char type,
        // double minXInterOffset = 0,
        // double maxXInterOffset = 0,
        // double minYInterOffset = 0,
        // double maxYInterOffset = 0,
        double interactionAngle)

    {
        //this->id = id;
        this->position = std::pair<double, double>(x, y);
        this->currentCapacity = currentCapacity;
        this->teamColor = teamColor;
        // this->type = type;
        // this->minXInteractableOffset = minXInterOffset;
        // this->maxXInteractableOffset = maxXInterOffset;
        // this->minYInteractableOffset = minYInterOffset;
        // this->maxYInteractableOffset = maxYInterOffset;
        this->interactionAngle = interactionAngle;
        this->isBackAligned = false;
    }

    private:
        int currentCapacity;

};