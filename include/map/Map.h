#pragma once
#include <vector>
#include <string>

#include "InteractionObject.h"
#include "GameElements.h"
#include "MapObstacle.h"
#include "Telemetry.h"

/*
    Representation of Map
*/

class Map {
    private:
        int nextElementID;

        void addObstacle(double upperLeftX, double upperLeftY, double lowerRightX, double lowerRightY) {
            mapElements.push_back(new Obstacle(nextElementID++, upperLeftX, upperLeftY, lowerRightX, lowerRightY));
        }

        void addTriball(double x, double y, char teamColor){
            mapElements.push_back(new InteractionObject(nextElementID++, x, y, teamColor, triBallInteractionAngle, true));
        }

        void addObject(double x, double y, char teamColor, double* interactionAngle) {
            mapElements.push_back(new InteractionObject(nextElementID++, x, y, teamColor, interactionAngle));
        }
        Telemetry* tm;


    public:
        Map(Telemetry* telemetry, AutoDrive* autoDrive) {
            tm = telemetry; 
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

            if (!autoDrive->isSkills)
            {
                // red
                addTriball(46.64 + xAdjust, 70.20 + yAdjust, 'R');                                  // element 10
                addTriball(65.95 + xAdjust, 93.77 + yAdjust, 'R');                                  // element 11
                addTriball(65.95 + xAdjust, 70.20 + yAdjust, 'R');                                  // element 12
                    // nuetral
                addTriball(70.20 + xAdjust, 129.11 + yAdjust, 'N');                                 // element 13
                addTriball(70.20 + xAdjust, 11.30 + yAdjust, 'N');                                  // element 14
                    // blue
                addTriball(74.46 + xAdjust, 70.20 + yAdjust, 'B');                                  // element 15
                addTriball(74.46 + xAdjust, 46.64 + yAdjust, 'B');                                  // element 16
                addTriball(93.77 + xAdjust, 70.20 + yAdjust, 'B');                                  // element 17
            }

            /*
            ----SKILLS TRIBALLS --------------------------------------------------------------------
            */
            else
            {
                    // red
                addTriball(44.64 + xAdjust, 70.22 + yAdjust, 'R');                                  // element 10
                addTriball(60.22 + xAdjust, 113.17 + yAdjust, 'R');                                 // element 11
                addTriball(60.22 + xAdjust, 93.79 + yAdjust, 'R');                                  // element 12
                addTriball(60.22 + xAdjust, 70.22 + yAdjust, 'R');                                  // element 13
                addTriball(60.22 + xAdjust, 46.66 + yAdjust, 'R');                                  // element 14
                addTriball(60.22 + xAdjust, 27.28 + yAdjust, 'R');                                  // element 15

                    // nuetral
                addTriball(70.20 + xAdjust, 129.13 + yAdjust, 'N');                                 // element 16
                addTriball(70.20 + xAdjust, 11.32 + yAdjust, 'N');                                  // element 17
            }

            /*
            ----------------------------------------------------------------------------------------
            */

            // load Zones
            
                // red
            addObject(22.89 / 2 + xAdjust, 129.11 + yAdjust, 'R', FORTY_FIVE);                      // element 18
            addObject(22.89 / 2 + xAdjust, 11.30  + yAdjust, 'R', ONE_THIRTY_FIVE);                 // element 19

                // blue
            addObject(122.76 + xAdjust, 129.11 + yAdjust, 'B', NEG_FORTY_FIVE);                     // element 20
            addObject(122.76 + xAdjust, 11.30 + yAdjust, 'B', NEG_ONE_THIRTY_FIVE);                 // element 21


            // corners-starting zones
            addObstacle(0 + xAdjust, 140.41 + yAdjust, 70.20 + xAdjust, 58.42 + yAdjust);           // element 22
            addObstacle(0 + xAdjust, 70.20 + yAdjust, 70.20 + xAdjust, 0 + yAdjust);                // element 23
            addObstacle(70.20 + xAdjust, 140.41 + yAdjust, 140.41 + xAdjust, 70.20 + yAdjust);      // element 24
            addObstacle(70.20 + xAdjust, 81.985 +yAdjust, 140.41 + xAdjust, 0 + yAdjust);           // element 25

            // triball goals--interaction point
                // red
            addObject(11.66 + xAdjust, 93.77 + yAdjust, 'R', ONE_EIGHTY);                           // element 26
            addObject(11.66 + xAdjust, 46.64 + yAdjust, 'R', ZERO);                                 // element 27
            addObject(23.32 + xAdjust, 81.985 + yAdjust, 'R', NINETY);                              // element 28
            addObject(23.32 + xAdjust, 70.20 + yAdjust, 'R', NINETY);                               // element 29
            addObject(23.32 + xAdjust, 58.41 + yAdjust, 'R', NINETY);                               // element 30

                //blue
            addObject(117.09 + xAdjust, 81.985 + yAdjust, 'B', TWO_SEVENTY);                        // element 31
            addObject(117.09 + xAdjust, 70.20 + yAdjust, 'B', TWO_SEVENTY);                         // element 32
            addObject(117.09 + xAdjust, 58.41 + yAdjust, 'B', TWO_SEVENTY);                         // element 33
            addObject(128.75 + xAdjust, 93.77 + yAdjust, 'B', ONE_EIGHTY);                          // element 34
            addObject(128.75 + xAdjust, 46.64 + yAdjust, 'B', ZERO);                                // element 35


        /*
            PLACE HOLDERS!!!                // TODO: Change Later once robot is built
        */
            // HighBar
            addObject(70.20 + xAdjust, 129.11 + yAdjust, 'B', triBallInteractionAngle);             // element 36
            addObject(70.20 + xAdjust, 11.30 + yAdjust, 'R', triBallInteractionAngle);              // element 37

            // Elevation Bar
            addObject(70.20 + xAdjust, 117.33 + yAdjust, 'B', triBallInteractionAngle);             // element 38
            addObject(70.20 + xAdjust, 23.08 + yAdjust, 'R', triBallInteractionAngle);              // element 39

        }



        // coordinates-angle-readjustment
        double xAdjust = -70.20;    // inches
        double yAdjust = -70.20;    // inches
        double headingAdjust = 90;  // degree

        double* triBallInteractionAngle = &(tm->odometry_heading);

        // for angles
         double* TWO_SEVENTY = new double(270 + headingAdjust);
         double* ONE_EIGHTY = new double(180 + headingAdjust);
         double* ONE_THIRTY_FIVE = new double (135 + headingAdjust);
         double* NINETY = new double (90 + headingAdjust);
         double* FORTY_FIVE = new double(45 + headingAdjust);

         double* NEG_TWO_SEVENTY = new double(-270 + headingAdjust);
         double* NEG_ONE_EIGHTY = new double(-180 + headingAdjust);
         double* NEG_ONE_THIRTY_FIVE = new double(-135 + headingAdjust);
         double* NEG_NINETY = new double(-90 + headingAdjust);
         double* NEG_FORTY_FIVE = new double(-45 + headingAdjust);

         double* ZERO = new double(0 + headingAdjust);


        // Map Elements
        std::vector<GameElement*> mapElements;

};