#pragma once
#include <tuple>
#include <GameElements.h>

class TriballGoal : private GameElement
{

    public:
    /// @brief Triball Goal constructor
    /// @param x1 X component of first interaction point
    /// @param y1 Y component of first interaction point
    /// @param interactionAngle1 
    /// @param x2 X component of second interaction point
    /// @param y2 Y component of second interaction point
    /// @param interactionAngle2 
    /// @param x3 X component of third interaction point
    /// @param y3 Y component of third interaction point
    /// @param interactionAngle3 
    /// @param x4 X component of fourth interaction point
    /// @param y4 Y component of fourth interaction point
    /// @param interactionAngle4 
    /// @param x5 X component of fifth interaction point
    /// @param y5 Y component of fifth interaction point
    /// @param interactionAngle5 
    TriballGoal(
        double x1,
        double y1,
        double interactionAngle1,
        double x2,
        double y2,
        double interactionAngle2,
        double x3,
        double y3,
        double interactionAngle3,
        double x4,
        double y4,
        double interactionAngle4,
        double x5,
        double y5,
        double interactionAngle5
    )

    // override getFunctions


    {
        this->point1 = std::make_tuple<double, double, double>(x1, y1, interactionAngle1);
        this->point2 = std::make_tuple<double, double, double>(x2, y2, interactionAngle2);
        this->point3 = std::make_tuple<double, double, double>(x3, y3, interactionAngle3);
        this->point4 = std::make_tuple<double, double, double>(x4, y4, interactionAngle4);
        this->point5 = std::make_tuple<double, double, double>(x5, y5, interactionAngle5);
    }

    private:
        std::tuple<double, double, double> point1;
        std::tuple<double, double, double> point2;
        std::tuple<double, double, double> point3;
        std::tuple<double, double, double> point4;
        std::tuple<double, double, double> point5;
};