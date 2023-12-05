#pragma once
#include <GameElements.h>

class ElevationBar : private GameElement
{
    public:
        ElevationBar(
            int id,
            double x,
            double y
        )

        {
            this->id = id;
            this->position = std::make_pair<double, double>(x, y);
        }
};