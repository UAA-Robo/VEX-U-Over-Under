#pragma once
#include <GameElements.h>

class Obstacle : private GameElement
{
    public:
        /// @brief Game Element Constructor
        /// @param id   Unique Identifier for Game Element
        /// @param ULX  Upper Left Corner X Position
        /// @param ULY  Upper Left Corner Y Position
        /// @param LRX  Lower Right Corner X Position
        /// @param LRY  Lower Right Corner Y Position
        Obstacle(
            int id,
            double ULX, 
            double ULY, 
            double LRX, 
            double LRY
        )

        {
            this->id = id;
            this->ULCorner = dbpair(ULX, ULY);
            this->LRCorner = dbpair(LRX, LRY);
        }



    private:
        dbpair ULCorner; // Upper Left Corner
        dbpair LRCorner; // Lower Right Corner 
};