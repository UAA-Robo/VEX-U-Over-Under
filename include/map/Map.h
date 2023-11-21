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
        void addTriball() {         // head-to-head
            // red side
            Triball Triball1(46.83 + xAdjust, 70.20 + yAdjust, false, 0);   /// add min offset for interaction
            Triball Triball2(65.95 + xAdjust, 93.77 + yAdjust, false, 0);   /// offset = robot_front - robot_center
            Triball Triball3(65.95 + xAdjust, 70.20 + yAdjust, false, 0);   /// replace interacion angle with odometry_heading + headingAdjust

            // middle bar
            Triball Triball4(70.20 + xAdjust, 129.11 + yAdjust, false, 0);
            Triball Triball5(70.20 + xAdjust, 11.30 + yAdjust, false, 0);

            // blue side
            Triball Triball6(74.46 + xAdjust, 70.20 + yAdjust, false, 0);
            Triball Triball7(74.46 + xAdjust, 46.64 + yAdjust, false, 0);
            Triball Triball8(93.77 + xAdjust, 70.20 + yAdjust, false, 0);

        }

        void addLoadZone() {
            // red side
            LoadZone LoadZone1(22.89 / 2 + xAdjust, 129.11 + yAdjust, R, 1, false, 45 + headingAdjust);
            LoadZone LoadZone2(22.89 / 2 + xAdjust, 11.30  + yAdjust, R, 1, false, 135 + headingAdjust);

            // blue side
            LoadZone LoadZone3(122.76 + xAdjust, 129.11 + yAdjust, B, 1, false, -45 + headingAdjust);
            LoadZone LoadZone4(122.76 + xAdjust, 11.30 + yAdjust, B,  1, false, -135 + headingAdjust);
        }

        void addGoal() {
            ;
        }

        void addHighBar() {
            // blue side
            HighBar HighBar1(70.20 + xAdjust, 129.11 + yAdjust, B, false, 45 + headingAdjust);  // fix heading

            // red side
            HighBar HighBar2(70.20 + xAdjust, 11.30 + yAdjust, R, false, 45 + headingAdjust);   // fix heading

        }

        void addLines() {
            ;    
        }



    public:
        Map() {
            nextElementID = 0;

            addObstacle(dbpair())
        }
    

    public:
        // coordinates-angle-readjustment
        double xAdjust = -70.20;    // inches
        double yAdjust = -70.20;    // inches
        double headingAdjust = 90;  // when rad: M_PI / 2

}