#include <GameElements.h>

class HighBar : private GameElement
{
    Triball(
        //int id,
        double x,
        double y,
        char teamColor,
        bool BackTowardsTarget = false,
        double interactionAngle = 0;)

    {
        //this->id = id;
        this->position = std::pair<double, double>(x, y);
        this->teamColor = teamColor;
        this->isBackAligned = BackTowardsTarget;
        this->interactionAngle = interactionAngle;
    }
};
