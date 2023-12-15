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


        /// @brief This method returns whether the bot is in its starting zone
        /// @param botX The bot's x_odometry position
        /// @param botY The bot's y_odometry position
        /// @return True if bot is in zone, else false
        bool inZone(
            double botX - xAdjust,
            double botY - yAdjust
        )
        {    
            if (botX > ULCorner.first && 
                botX < LRCorner.first &&  
                botY < ULCorner.second && 
                botY > LRCorner.second)
            {
                return true;
            }
            return false;
        }

    private:
        dbpair ULCorner; // Upper Left Corner
        dbpair LRCorner; // Lower Right Corner 
};