#pragma once
#include <vector>
#include <string>

#include "InteractionObject.h"
#include "GameElements.h"
#include "MapObstacle.h"

/*
    Representation of Map
*/

class Map {
    private:
        int nextElementID;

        void addObstacle(double upperLeftX, double upperLeftY, double lowerRightX, double lowerRightY) {
            mapElements.push_back(new Obstacle(nextElementID++, upperLeftX, upperLeftY, lowerRightX, lowerRightY));
        }

        void addTriball(double x, double y, char teamColor, double interactionAngle) {
            mapElements.push_back(new InteractionObject(nextElementID++, x, y, teamColor, interactionAngle));
        }

        void addObject(double x, double y, char teamColor, double interactionAngle) {
            mapElements.push_back(new InteractionObject(nextElementID++, x, y, teamColor, interactionAngle));
        }


    public:
        Map(Telemetry* tm, UserDrive* userDrive) {
            nextElementID = 0;

            // outer walls
            addObstacle(0 + xAdjust, 140.40 + yAdjust, 0 + xAdjust, 0 + yAdjust);                   // element 0                                                           
            addObstacle(0 + xAdjust, 140.40 + yAdjust, 140.40 + xAdjust, 140.40 + yAdjust);         // element 1
            addObstacle(0 + xAdjust, 0 + yAdjust, 140.40 + xAdjust, 0 + yAdjust);                   // element 2
            addObstacle(140.40 + xAdjust, 140.40 + yAdjust, 0 + xAdjust, 140.40 + yAdjust);         // element 3

            // inner walls
            addObstacle(45.61 + xAdjust, 118.51 + yAdjust, 94.79 + xAdjust, 118.51 + yAdjust);      // element 4
            addObstacle(45.61 + xAdjust, 118.51 + yAdjust, 94.79 + xAdjust, 118.51 + yAdjust);      // element 5
            addObstacle(69.01 + xAdjust, 118.51 + yAdjust, 69.01 + xAdjust, 21.89 + yAdjust);       // element 6
            addObstacle(71.39 + xAdjust, 118.51 + yAdjust, 71.39 + xAdjust, 21.89 + yAdjust);       // element 7
            addObstacle(45.61 + xAdjust, 24.26 + yAdjust, 94.79 + xAdjust, 24.26 + yAdjust);        // element 8
            addObstacle(45.61 + xAdjust, 21.89 + yAdjust, 94.79 + xAdjust, 21.89 + yAdjust);        // element 9

            /*
            ----HEAD TO HEAD TRIBALLS --------------------------------------------------------------
            */
            /////////////////////// Change interaction angle to tm->odometry_heading + headingAdjust

            // if (!isSkills)

                // red
            addTriball(46.64 + xAdjust, 70.20 + yAdjust, 'R', tm->odometry_heading + headingAdjust);// element 10
            addTriball(65.95 + xAdjust, 93.77 + yAdjust, 'R', tm->odometry_heading + headingAdjust);// element 11
            addTriball(65.95 + xAdjust, 70.20 + yAdjust, 'R', tm->odometry_heading + headingAdjust);// element 12
                // nuetral
            addTriball(70.20 + xAdjust, 129.11 + yAdjust, 'N', tm->odometry_heading + headingAdjust);// element 13
            addTriball(70.20 + xAdjust, 11.30 + yAdjust, 'N', tm->odometry_heading + headingAdjust);// element 14
                // blue
            addTriball(74.46 + xAdjust, 70.20 + yAdjust, 'B', tm->odometry_heading + headingAdjust);// element 15
            addTriball(74.46 + xAdjust, 46.64 + yAdjust, 'B', tm->odometry_heading + headingAdjust);// element 16
            addTriball(93.77 + xAdjust, 70.20 + yAdjust, 'B', tm->odometry_heading + headingAdjust);// element 17


            /*
            ----SKILLS TRIBALLS --------------------------------------------------------------------
            */
            // else

                // red
            addTriball(44.64 + xAdjust, 70.22 + yAdjust, 'R', tm->odometry_heading + headingAdjust);// element 10
            addTriball(60.22 + xAdjust, 113.17 + yAdjust, 'R', tm->odometry_heading + headingAdjust);// element 11
            addTriball(60.22 + xAdjust, 93.79 + yAdjust, 'R', tm->odometry_heading + headingAdjust);// element 12
            addTriball(60.22 + xAdjust, 70.22 + yAdjust, 'R', tm->odometry_heading + headingAdjust);// element 13
            addTriball(60.22 + xAdjust, 46.66 + yAdjust, 'R', tm->odometry_heading + headingAdjust);// element 14
            addTriball(60.22 + xAdjust, 27.28 + yAdjust, 'R', tm->odometry_heading + headingAdjust);// element 15

                // nuetral
            addTriball(70.20 + xAdjust, 129.13 + yAdjust, 'N', tm->odometry_heading + headingAdjust);// element 16
            addTriball(70.20 + xAdjust, 11.32 + yAdjust, 'N', tm->odometry_heading + headingAdjust);// element 17

            /*
            ----------------------------------------------------------------------------------------
            */

            // load Zones
            
                // red
            addObject(22.89 / 2 + xAdjust, 129.11 + yAdjust, 'R', 45 + headingAdjust);              // element 18
            addObject(22.89 / 2 + xAdjust, 11.30  + yAdjust, 'R', 135 + headingAdjust);             // element 19

                // blue
            addObject(122.76 + xAdjust, 129.11 + yAdjust, 'B', -45 + headingAdjust);                // element 20
            addObject(122.76 + xAdjust, 11.30 + yAdjust, 'B', -135 + headingAdjust);                // element 21


            // corners-starting zones
            addObstacle(0 + xAdjust, 140.41 + yAdjust, 70.20 + xAdjust, 58.42 + yAdjust);           // element 22
            addObstacle(0 + xAdjust, 70.20 + yAdjust, 70.20 + xAdjust, 0 + yAdjust);                // element 23
            addObstacle(70.20 + xAdjust, 140.41 + yAdjust, 140.41 + xAdjust, 70.20 + yAdjust);      // element 24
            addObstacle(70.20 + xAdjust, 81.985 +yAdjust, 140.41 + xAdjust, 0 + yAdjust);           // element 25

            // triball goals--interaction point
                // red
            addObject(11.66 + xAdjust, 93.77 + yAdjust, 'R', 180 + headingAdjust);                  // element 26
            addObject(11.66 + xAdjust, 46.64 + yAdjust, 'R', 180 + headingAdjust);                  // element 27
            addObject(23.32 + xAdjust, 81.985 + yAdjust, 'R', 90 + headingAdjust);                  // element 28
            addObject(23.32 + xAdjust, 70.20 + yAdjust, 'R', 90 + headingAdjust);                   // element 29
            addObject(23.32 + xAdjust, 58.41 + yAdjust, 'R', 90 + headingAdjust);                   // element 30

                //blue
            addObject(117.09 + xAdjust, 81.985 + yAdjust, 'B', 270 + headingAdjust);                // element 31
            addObject(117.09 + xAdjust, 70.20 + yAdjust, 'B', 270 + headingAdjust);                 // element 32
            addObject(117.09 + xAdjust, 58.41 + yAdjust, 'B', 270 + headingAdjust);                 // element 33
            addObject(128.75 + xAdjust, 93.77 + yAdjust, 'B', 180 + headingAdjust);                 // element 34
            addObject(128.75 + xAdjust, 46.64 + yAdjust, 'B', 180 + headingAdjust);                 // element 35


        /*
            PLACE HOLDERS!!!                // TODO: Change Later once robot is built
        */
            // HighBar
            addObject(70.20 + xAdjust, 129.11 + yAdjust, 'B', tm->odometry_heading + headingAdjust);// element 36
            addObject(70.20 + xAdjust, 11.30 + yAdjust, 'R', tm->odometry_heading + headingAdjust); // element 37

            // Elevation Bar
            addObject(70.20 + xAdjust, 117.33 + yAdjust, 'B', tm->odometry_heading + headingAdjust);// element 38
            addObject(70.20 + xAdjust, 23.08 + yAdjust, 'R', tm->odometry_heading + headingAdjust); // element 39

        }



        // coordinates-angle-readjustment
        double xAdjust = -70.20;    // inches
        double yAdjust = -70.20;    // inches
        double headingAdjust = -90;  // degree

        // Map Elements
        std::vector<GameElement*> mapElements;

};