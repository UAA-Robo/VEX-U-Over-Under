#pragma once
#include <GameElements.h>

class Obstacle : private GameElement
{
    public:
    
        Obstacle(
            double ULX, 
            double ULY, 
            double LRX, 
            double LRY,
        )

        {
            this->ULCorner = dbpair(ULX, ULY);
            this->LRCorner = dbpair(LRX, LRY);
        }



    private:
        dbpair ULCorner; // Upper Left Corner
        dbpair LRCorner; // Lower Right Corner 
};