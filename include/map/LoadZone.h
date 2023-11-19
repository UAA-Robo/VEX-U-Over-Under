#include <GameElements.h>

class LoadZone : private
{
    GameElement(
        //int id,
        double x,
        double y,
        double rotation,
        // char teamColor,
        // char type,
        bool BackTowardsTarget = false,
        // double minXInterOffset = 0,
        // double maxXInterOffset = 0,
        // double minYInterOffset = 0,
        // double maxYInterOffset = 0,
        double interactionAngle = 0;)
    {
        //this->id = id;
        this->position = std::pair<double, double>(x, y);
        this->rotation = rotation;
        // this->teamColor = teamColor;
        // this->type = type;
        this->isBackAligned = BackTowardsTarget;
        // this->minXInteractableOffset = minXInterOffset;
        // this->maxXInteractableOffset = maxXInterOffset;
        // this->minYInteractableOffset = minYInterOffset;
        // this->maxYInteractableOffset = maxYInterOffset;
        this->interactionAngle = interactionAngle;
    }
}