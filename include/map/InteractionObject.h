#pragma once
#include "GameElements.h"

class InteractionObject : public GameElement
{
    private:
        std::pair<double, double> position;
        char teamColor;
        double* interactionAngle;

    public:
        /// @brief Triball Element Constructor
        /// @param id Unique Identifier for Game Element
        /// @param x X Component Position
        /// @param y Y Component Position
        /// @param teamColor If an element is part of a specific team then this is set to that team's identifier
        /// @param interactionAngle The angle required for the bot to interact with the Game Element.
        InteractionObject(int id, double x, double y, char teamColor, double* interactionAngle, bool isBackTowardsTarget = false) : 
        GameElement(id, isBackTowardsTarget) {
            this->position = std::pair<double, double>(x, y);
            this->teamColor = teamColor;
            this->interactionAngle = interactionAngle;
        };

    // Setters

        //  position
    void SetPosition(double x, double y) { this->position = std::pair<double, double>(x, y); }
    void SetPosition(std::pair<double, double> position) { this->position = position; }

        // teamColor
    void SetTeamColor(char color) { this->teamColor = color; }

    // Getters

        // position
    std::pair<double, double> GetPosition() { return position; }
    std::pair<double, double> GetPositionWithMinOffset()
    {
        return {position.first + minXInteractableOffset, position.second + minYInteractableOffset};
    }
    double GetXPosition() { return position.first; }
    double GetYPosition() { return position.second; }

    dbpair test() { return position + position; }

        // teamColor
    char GetTeamColor() { return teamColor; }

};