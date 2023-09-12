#pragma once
#include <vector>
#include <string>

#include "LoadZone.h"
#include "TriballGoal.h"
#include "Triballs.h"
#include "GameElements.h"

/*
    Representation of Map
*/

class Map {
    private:
        void addObstacle() {
            ;
        }
        void addTriball() {
            ;
        }

        void addLoadZone() {
            ;
        }

        void addGoal() {
            ;
        }

    public:
        Map() {
            nextElementID = 0;

            addObstacle(dbpair())
        }

}