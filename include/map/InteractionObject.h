#pragma once
#include "GameElements.h"

class InteractionObject : public GameElement
{
    private:
        std::pair<double, double> position;
        char team_color;
        double* interaction_angle;

    public:
        /// @brief Triball Element Constructor
        /// @param id Unique Identifier for Game Element
        /// @param x X Component Position
        /// @param y Y Component Position
        /// @param team_color If an element is part of a specific team then this is set to that team's identifier
        /// @param interaction_angle The angle required for the bot to interact with the Game Element.
        InteractionObject(int id, double x, double y, char team_color, double* interaction_angle, bool IS_BACK_TOWARDS_TARGET = false) : 
        GameElement(id, IS_BACK_TOWARDS_TARGET) {
            this->position = std::pair<double, double>(x, y);
            this->team_color = team_color;
            this->interaction_angle = interaction_angle;
        };


    // Getters
    std::pair<double, double> get_position() { return position; }
    double* get_interaction_angle() { return interaction_angle; }

};