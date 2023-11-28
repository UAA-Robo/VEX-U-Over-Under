#pragma once
#include <GameElements.h>

class Line : private GameElement
{
    public:
        Line(
            double startX, 
            double endX, 
            double y,
            char teamColor,
            bool canCross
        )

        {
            this->startX    = startX;
            this->endX      = endX;
            this->y         = y;
            this->teamColor = teamColor;
            //this->canCross  = canCross;
        }




    private:
        /// @brief canInteract - Determines whether the line can be crossed
        double startX;
        double endX;
        //bool canCross;

};