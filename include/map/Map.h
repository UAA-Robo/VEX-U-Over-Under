#pragma once
#include <vector>
#include <string>

#include "LoadZone.h"
#include "TriballGoal.h"
#include "Triballs.h"
#include "GameElements.h"
#include "MapObstacles.h"

/*
    Representation of Map
*/



class Map {
    private:
        void addObstacle() {
            // outer walls
            Obstacle Wall1(0 + xAdjust, 140.40 + yAdjust, 0 + xAdjust, 0 + yAdjust);
            Obstacle Wall2(0 + xAdjust, 140.40 + yAdjust, 140.40 + xAdjust, 140.40 + yAdjust);
            Obstacle Wall3(0 + xAdjust, 0 + yAdjust, 140.40 + xAdjust, 0 + yAdjust);
            Obstacle Wall4(140.40 + xAdjust, 140.40 + yAdjust, 0 + xAdjust, 140.40 + yAdjust);

            // inner walls
            Obstacle Wall5(45.61 + xAdjust, 118.51 + yAdjust, 94.79 + xAdjust, 118.51 + yAdjust);
            Obstacle Wall6(45.61 + xAdjust, 118.51 + yAdjust, 94.79 + xAdjust, 118.51 + yAdjust);
            Obstacle Wall7(69.01 + xAdjust, 118.51 + yAdjust, 69.01 + xAdjust, 21.89 + yAdjust);
            Obstacle Wall8(71.39 + xAdjust, 118.51 + yAdjust, 71.39 + xAdjust, 21.89 + yAdjust);

            Obstacle Wall9(45.61 + xAdjust, 24.26 + yAdjust, 94.79 + xAdjust, 24.26 + yAdjust);
            Obstacle Wall10(45.61 + xAdjust, 21.89 + yAdjust, 94.79 + xAdjust, 21.89 + yAdjust);

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

        void addTriball() {         // skills
            Triball Triball1()
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
            HighBar HighBar1(70.20 + xAdjust, 129.11 + yAdjust, B, false);  // fix heading

            // red side
            HighBar HighBar2(70.20 + xAdjust, 11.30 + yAdjust, R, false);   // fix heading

        }

        void addLines() {
            // red side
            Line Line1(0 + xAdjust, 70.20 + xAdjust, 70.20 + yAdjust, R, true);
            Line Line2(0 + xAdjust, 70.20 + xAdjust, 58.42 + yAdjust, R, false);

            // blue side
            Line Line3(70.20 + xAdjust, 140.41 + xAdjust, 81.98 + yAdjust, B, false);
            Line Line4(70.20 + xAdjust, 140.41 + xAdjust, 70.20 + yAdjust, B, true);
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