#pragma once
#include "GameElements.h"
#include "Telemetry.h"
#include "RobotConfig.h"

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
    double* get_interaction_angle() { return interaction_angle; }
    std::pair<double, double> get_position() { return position; }
   
    // std::pair<double, double> get_position_with_offset()
    // {
    //     std::pair<double, double> new_position(this->get_position());
    //     double element_interaction_angle = *(this->get_interaction_angle());

    // // adjusted for heading_adjust, original get_interaction_angle = ...
    // if (element_interaction_angle == tm->get_current_heading()) { /* Any angle */}
    // else if (element_interaction_angle == 90) {     // ZERO
    //     new_position.second -= rc->DRIVETRAIN_RADIUS;
    // }
    // else if (element_interaction_angle == 135) {    // FORTY_FIVE
    //     new_position.first += rc->DRIVETRAIN_RADIUS / sqrt(2);
    //     new_position.second -= rc->DRIVETRAIN_RADIUS / sqrt(2);
    // }
    // else if (element_interaction_angle == 180) {    // NINETY
    //     new_position.first += rc->DRIVETRAIN_RADIUS;
    // }
    // else if (element_interaction_angle == 225) {    // ONE_THIRTY_FIVE
    //     new_position.first += rc->DRIVETRAIN_RADIUS / sqrt(2);
    //     new_position.second += rc->DRIVETRAIN_RADIUS / sqrt(2);
    // }
    // else if (element_interaction_angle == 270) {    // ONE_EIGHTY
    //     new_position.second += rc->DRIVETRAIN_RADIUS;
    // }
    // else if (element_interaction_angle == 360) {    // TWO_SEVENTY
    //     new_position.first -= rc->DRIVETRAIN_RADIUS;
    // }
    // else if (element_interaction_angle == 45) {    // NEG_FORTY_FIVE
    //     new_position.first += rc->DRIVETRAIN_RADIUS / sqrt(2);
    //     new_position.second -= rc->DRIVETRAIN_RADIUS / sqrt(2);
    // }
    // else if (element_interaction_angle == -45) {    // NEG_ONE_THIRTY_FIVE
    //     new_position.first -= rc->DRIVETRAIN_RADIUS / sqrt(2);
    //     new_position.second -= rc->DRIVETRAIN_RADIUS / sqrt(2);
    // }

    // return new_position;
    // }

};