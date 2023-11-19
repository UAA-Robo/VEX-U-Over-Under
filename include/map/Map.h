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
            // red side
            Triball Triball1(46.83 + xAdjust, 70.20 + yAdjust, false, 0);   /// add min offset for interaction
            Triball Triball2(65.95 + xAdjust, 93.77 + yAdjust, false, 0);   /// offset = robot_front - robot_center
            Triball Triball3(65.95 + xAdjust, 70.20 + yAdjust, false, 0);   /// add team color

            // middle bar
            Triball Triball4(70.20 + xAdjust, 129.11 + yAdjust, false, 0);
            Triball Triball5(70.20 + xAdjust, 11.30 + yAdjust, false, 0);

            // blue side
            Triball Triball6(74.46 + xAdjust, 70.20 + yAdjust, false, 0);
            Triball Triball7(74.46 + xAdjust, 46.64 + yAdjust, false, 0);
            Triball Triball8(93.77 + xAdjust, 70.20 + yAdjust, false, 0);

        }

        void addLoadZone() {
            LoadZone LoadZone1(22.89 / 2 + xAdjust, 129.11 + yAdjust, -45 + headingAdjust, false, )
            
        }

        void addGoal() {
            ;
        }

    public:
        Map() {
            nextElementID = 0;

            addObstacle(dbpair())
        }
    

    private:
        // coordinates-angle-readjustment
        double xAdjust = -70.20;    // inches
        double yAdjust = -70.20;    // inches
        double headingAdjust = 90;  // when rad: M_PI / 2

}