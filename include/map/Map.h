#pragma once
#include <vector>
#include <string>

#include "InteractionObject.h"
#include "GameElements.h"
#include "MapObstacle.h"
#include "Telemetry.h"
#include "RobotConfig.h"
#include "Buffer.h"
#include "Region.h"

/*
    Representation of Map
*/

class Map {
    private:
        int nextElementID;
        RobotConfig *rc;
        Telemetry* tm;
        bool skills;

        /// @brief Adds a buffer to the map.
        /// @param upper_left_corner The upper left corner coordinates.
        /// @param bottom_right_corner The bottom right corner coordinates.
        void add_buffer(
        std::pair<double, double> upper_left_corner,
        std::pair<double, double> bottom_right_corner) {
            Buffer bf(upper_left_corner, bottom_right_corner);
            buffers.push_back(&bf);
        }

        /// @brief Adds a rectangular region to the map.
        /// @param upper_left_corner The upper left corner coordinates.
        /// @param bottom_right_corner The bottom right corner coordinates.
        /// @param upper_critical_point The next critical point in a clockwise direction.
        /// @param lower_critical_point The current critical point in a clockwise direction.
        void add_simple_region(
        std::pair<double, double> upper_left_corner,
        std::pair<double, double> bottom_right_corner,
        std::pair<double, double> upper_critical_point,
        std::pair<double, double> lower_critical_point
        ) {
            SimpleRegion* sr = new SimpleRegion( // Yo props to Wa for the ++ thing, very clever
                next_region_id++, upper_left_corner, bottom_right_corner,
                upper_critical_point, lower_critical_point);
            regions.push_back(sr);
        }

        /// @brief Adds an L-shaped region to the map.
        /// @param upper_left_corner_1 The upper left corner coordinates of the first rectangle that
        ///                            makes up the L-shape.
        /// @param bottom_right_corner_1 The bottom right corner coordinates of the first rectangle
        ///                              that makes up the L-shape.
        /// @param upper_left_corner_2 The upper left corner coordinates of the second rectangle
        ///                            that makes up the L-shape.
        /// @param bottom_right_corner_2 The bottom right corner coordinates of the second rectangle
        ///                              that makes up the L-shape.
        /// @param upper_critical_point The next critical point in a clockwise direction.
        /// @param lower_critical_point The current critical point in a clockwise direction.
        void add_composite_region(
        std::pair<double, double> upper_left_corner_1,
        std::pair<double, double> bottom_right_corner_1,
        std::pair<double, double> upper_left_corner_2,
        std::pair<double, double> bottom_right_corner_2,
        std::pair<double, double> upper_critical_point,
        std::pair<double, double> lower_critical_point
        ) {
            CompositeRegion* cr = new CompositeRegion(
                next_region_id++, upper_left_corner_1, bottom_right_corner_1,
                upper_left_corner_2, bottom_right_corner_2,
                upper_critical_point, lower_critical_point);
            regions.push_back(cr);
        }

        void addObstacle(double upperLeftX, double upperLeftY, double lowerRightX, double lowerRightY) {
            mapElements.push_back(new Obstacle(nextElementID++, upperLeftX, upperLeftY, lowerRightX, lowerRightY));
        }

        void addTriball(double x, double y, char teamColor){
            mapElements.push_back(new InteractionObject(nextElementID++, x, y, teamColor, triBallInteractionAngle, true));
        }

        void addObject(double x, double y, char teamColor, double* interactionAngle) {
            mapElements.push_back(new InteractionObject(nextElementID++, x, y, teamColor, interactionAngle));
        }


    public:
        Map(Telemetry* telemetry, RobotConfig *robotConfig, bool isSkills) {
            tm = telemetry;
            rc = robotConfig;
            nextElementID = 0;
            skills = isSkills;
            double drive_train_adjustment = rc->DRIVETRAINWIDTH * sqrt(2) / 2;

            // outer walls
            addObstacle(0 + x_adjust, 140.40 + y_adjust, 0 + x_adjust, 0 + y_adjust);                   // element 0                                                           
            addObstacle(0 + x_adjust, 140.40 + y_adjust, 140.40 + x_adjust, 140.40 + y_adjust);         // element 1
            addObstacle(0 + x_adjust, 0 + y_adjust, 140.40 + x_adjust, 0 + y_adjust);                   // element 2
            addObstacle(140.40 + x_adjust, 140.40 + y_adjust, 0 + x_adjust, 140.40 + y_adjust);         // element 3

            // inner walls
            addObstacle(45.61 + x_adjust, 118.51 + y_adjust, 94.79 + x_adjust, 118.51 + y_adjust);      // element 4
            addObstacle(45.61 + x_adjust, 118.51 + y_adjust, 94.79 + x_adjust, 118.51 + y_adjust);      // element 5
            addObstacle(69.01 + x_adjust, 118.51 + y_adjust, 69.01 + x_adjust, 21.89 + y_adjust);       // element 6
            addObstacle(71.39 + x_adjust, 118.51 + y_adjust, 71.39 + x_adjust, 21.89 + y_adjust);       // element 7
            addObstacle(45.61 + x_adjust, 24.26 + y_adjust, 94.79 + x_adjust, 24.26 + y_adjust);        // element 8
            addObstacle(45.61 + x_adjust, 21.89 + y_adjust, 94.79 + x_adjust, 21.89 + y_adjust);        // element 9

            /*
            ----HEAD TO HEAD TRIBALLS --------------------------------------------------------------
            */

            if (skills)
            {
                // red
                addTriball(46.64 + x_adjust, 70.20 + y_adjust, 'R');                                  // element 10
                addTriball(65.95 + x_adjust, 93.77 + y_adjust, 'R');                                  // element 11
                addTriball(65.95 + x_adjust, 70.20 + y_adjust, 'R');                                  // element 12
                    // nuetral
                addTriball(70.20 + x_adjust, 129.11 + y_adjust, 'N');                                 // element 13
                addTriball(70.20 + x_adjust, 11.30 + y_adjust, 'N');                                  // element 14
                    // blue
                addTriball(74.46 + x_adjust, 70.20 + y_adjust, 'B');                                  // element 15
                addTriball(74.46 + x_adjust, 46.64 + y_adjust, 'B');                                  // element 16
                addTriball(93.77 + x_adjust, 70.20 + y_adjust, 'B');                                  // element 17
            }

            /*
            ----SKILLS TRIBALLS --------------------------------------------------------------------
            */
            else
            {
                    // red
                addTriball(44.64 + x_adjust, 70.22 + y_adjust, 'R');                                  // element 10
                addTriball(60.22 + x_adjust, 113.17 + y_adjust, 'R');                                 // element 11
                addTriball(60.22 + x_adjust, 93.79 + y_adjust, 'R');                                  // element 12
                addTriball(60.22 + x_adjust, 70.22 + y_adjust, 'R');                                  // element 13
                addTriball(60.22 + x_adjust, 46.66 + y_adjust, 'R');                                  // element 14
                addTriball(60.22 + x_adjust, 27.28 + y_adjust, 'R');                                  // element 15

                    // nuetral
                addTriball(70.20 + x_adjust, 129.13 + y_adjust, 'N');                                 // element 16
                addTriball(70.20 + x_adjust, 11.32 + y_adjust, 'N');                                  // element 17
            }

            /*
            ----------------------------------------------------------------------------------------
            */

            // load Zones
            
                // red
            addObject(22.89 / 2 + x_adjust, 129.11 + y_adjust, 'R', FORTY_FIVE);                      // element 18
            addObject(22.89 / 2 + x_adjust, 11.30  + y_adjust, 'R', ONE_THIRTY_FIVE);                 // element 19

                // blue
            addObject(122.76 + x_adjust, 129.11 + y_adjust, 'B', NEG_FORTY_FIVE);                     // element 20
            addObject(122.76 + x_adjust, 11.30 + y_adjust, 'B', NEG_ONE_THIRTY_FIVE);                 // element 21


            // corners-starting zones
            addObstacle(0 + x_adjust, 140.41 + y_adjust, 70.20 + x_adjust, 58.42 + y_adjust);           // element 22
            addObstacle(0 + x_adjust, 70.20 + y_adjust, 70.20 + x_adjust, 0 + y_adjust);                // element 23
            addObstacle(70.20 + x_adjust, 140.41 + y_adjust, 140.41 + x_adjust, 70.20 + y_adjust);      // element 24
            addObstacle(70.20 + x_adjust, 81.985 +y_adjust, 140.41 + x_adjust, 0 + y_adjust);           // element 25

            // triball goals--interaction point
                // red
            addObject(11.66 + x_adjust, 93.77 + y_adjust, 'R', ONE_EIGHTY);                           // element 26
            addObject(11.66 + x_adjust, 46.64 + y_adjust, 'R', ZERO);                                 // element 27
            addObject(23.32 + x_adjust, 81.985 + y_adjust, 'R', NINETY);                              // element 28
            addObject(23.32 + x_adjust, 70.20 + y_adjust, 'R', NINETY);                               // element 29
            addObject(23.32 + x_adjust, 58.41 + y_adjust, 'R', NINETY);                               // element 30

                //blue
            addObject(117.09 + x_adjust, 81.985 + y_adjust, 'B', TWO_SEVENTY);                        // element 31
            addObject(117.09 + x_adjust, 70.20 + y_adjust, 'B', TWO_SEVENTY);                         // element 32
            addObject(117.09 + x_adjust, 58.41 + y_adjust, 'B', TWO_SEVENTY);                         // element 33
            addObject(128.75 + x_adjust, 93.77 + y_adjust, 'B', ONE_EIGHTY);                          // element 34
            addObject(128.75 + x_adjust, 46.64 + y_adjust, 'B', ZERO);                                // element 35


        /*
            PLACE HOLDERS!!!                // TODO: Change Later once robot is built
        */
            // HighBar
            addObject(70.20 + x_adjust, 129.11 + y_adjust, 'B', triBallInteractionAngle);             // element 36
            addObject(70.20 + x_adjust, 11.30 + y_adjust, 'R', triBallInteractionAngle);              // element 37

            // Elevation Bar
            addObject(70.20 + x_adjust, 117.33 + y_adjust, 'B', triBallInteractionAngle);             // element 38
            addObject(70.20 + x_adjust, 23.08 + y_adjust, 'R', triBallInteractionAngle);              // element 39

        


            /*
            ----- REGIONS --------------------------------------------------------------------------
            */
            /*
                .------------------------------. .--------------------------------.
                [          BLUE SIDE           ] [            RED SIDE            ]
                '------------------------------' '--------------------------------'
                 __________________________________________________________________
                |   Top               |                     |                      |
                |   Left         X    |   Top Way Region    |     X       Top      |
                |   L-Region          |   3                 |  4          Right    |
                |           __________|_____________________|___________  L-Region |
                |        2 |          '---------| |---------'           |          |
                |          |                    | |                     |          |
                |          |     X         1    | |  5            X     |          |
                |__________|                    | |                     |__________|
                |**********|                    | |                     |**********|
                |**********|                    | |                     |**********|
                |**********|    Left Offensive  | |    Right Offensive  |**********|
                |**********|    Region          | |    Region           |**********|
                |**********|                    | |                     |**********|
                |**********|                    | |                     |**********|
                |__________|                    | |                     |__________|
                |          |                    | |                     |          |
                |   0      |     X              | |               X     |  6       |
                |          |           _________| |__________           |          |
                |          |___________'--------'-'---------'___________|          |
                |    Lower            |  7                   |          Lower      |
                |    Left        X    |   Bottom Way Region  |    X     Right      |
                |    L-Region         |                      |          L-Region   |
                |_____________________|______________________|_____________________|
            */

            add_composite_region( // Lower Left L-Region
                {0.0 + x_adjust, 46.64 + y_adjust},
                {23.08 + x_adjust, 0.0 + y_adjust},
                {22.89 + x_adjust, 23.08 + y_adjust},
                {46.83 + x_adjust, 0.0 + y_adjust},
                {34.955 + x_adjust, 34.86 + y_adjust},
                {34.955 + x_adjust, 11.445 + y_adjust}
            );
            add_simple_region( // Left Offensive Region
                {22.89 + x_adjust, 117.52 + y_adjust},
                {70.20 + x_adjust, 22.89 + y_adjust},
                {34.955 + x_adjust, 105.55 + y_adjust},
                {34.955 + x_adjust, 34.86 + y_adjust}
            );
            add_composite_region( // Top Left L-Region
                {0.0 + x_adjust, 140.41 + y_adjust},
                {23.08 + x_adjust, 93.77 + y_adjust},
                {22.89 + x_adjust, 140.41 + y_adjust},
                {46.83 + x_adjust, 117.33 + y_adjust},
                {34.955 + x_adjust, 128.965 + y_adjust},
                {34.955 + x_adjust, 105.55 + y_adjust}   
            );
            add_simple_region( // Top Way Region
                {46.83 + x_adjust, 140.41 + y_adjust},
                {93.58 + x_adjust, 117.33 + y_adjust},
                {105.55 + x_adjust, 128.965 + y_adjust},
                {34.955 + x_adjust, 128.965 + y_adjust}
            );
            add_composite_region( // Top Right L-Region
                {93.58 + x_adjust, 140.41 + y_adjust},
                {117.52 + x_adjust, 117.33 + y_adjust},
                {117.33 + x_adjust, 140.41 + y_adjust},
                {140.41 + x_adjust, 93.77 + y_adjust},
                {105.55 + x_adjust, 105.645 + y_adjust},
                {105.55 + x_adjust, 128.965 + y_adjust}
            );
            add_simple_region( // Right Offensive Region
                {70.20 + x_adjust, 117.52 + y_adjust},
                {117.52 + x_adjust, 22.89 + y_adjust},
                {105.55 + x_adjust, 34.86 + y_adjust},
                {105.55 + x_adjust, 105.645 + y_adjust}
            );
            add_composite_region( // Lower Right L-Region
                {93.58 + x_adjust, 23.08 + y_adjust},
                {117.52 + x_adjust, 0.0 + y_adjust},
                {117.33 + x_adjust, 46.64 + y_adjust},
                {140.41 + x_adjust, 0.0 + y_adjust},
                {105.55 + x_adjust, 11.445 + y_adjust},
                {105.55 + x_adjust, 34.86 + y_adjust}
            );
            add_simple_region( // Bottom Way Region
                {46.83 + x_adjust, 23.08 + y_adjust},
                {93.58 + x_adjust, 0.0 + y_adjust},
                {34.955 + x_adjust, 11.445 + y_adjust},
                {105.55 + x_adjust, 11.445 + y_adjust}
            );

            /*
            ---- BUFFERS ---------------------------------------------------------------------------
            */

            add_buffer( // Blue Goal
                { // Upper Left Corner
                    0.0 - drive_train_adjustment + x_adjust,
                    97.77 + drive_train_adjustment + y_adjust
                },
                { // Bottom Right Corner
                    27.08 + drive_train_adjustment + x_adjust,
                    42.64 - drive_train_adjustment + y_adjust
                }
            );
            add_buffer( // Red Goal
                { // Upper Left Corner
                    113.33 - drive_train_adjustment + x_adjust,
                    97.77 + drive_train_adjustment + y_adjust
                },
                { // Bottom Right Corner
                    140.41 + drive_train_adjustment + x_adjust,
                    42.64 - drive_train_adjustment + y_adjust
                }
            );
            add_buffer( // Bottom Barrier Bar
                { // Upper Left Corner
                    45.83 - drive_train_adjustment + x_adjust,
                    24.58 + drive_train_adjustment + y_adjust
                },
                { // Bottom Right Corner
                    94.58 + drive_train_adjustment + x_adjust,
                    21.39 - drive_train_adjustment + y_adjust
                }
            );
            add_buffer( // Top Barrier Bar
                { // Upper Left Corner
                    45.83 - drive_train_adjustment + x_adjust,
                    119.02 + drive_train_adjustment + y_adjust
                },
                { // Bottom Right Corner
                    94.58 + drive_train_adjustment + x_adjust,
                    115.83 - drive_train_adjustment +y_adjust
                }
            );
            add_buffer( // Middle Barrier Bar
                { // Upper Left Corner
                    68.7 - drive_train_adjustment + x_adjust,
                    119.02 + drive_train_adjustment + y_adjust
                },
                { // Bottom Right Corner
                    71.7 + drive_train_adjustment + x_adjust,
                    21.39 - drive_train_adjustment + y_adjust
                }
            );
        }

        /// @brief Retrieves the ID of the region for the given position
        /// @param position A coordinate pair for a position
        /// @return The integer ID of the container region, -1 if none
        int in_which_region(std::pair<double, double> position, int last_region, int direction) {
            for (int i = 0; i < regions.size(); ++i) {
                if (regions[i]->is_critical_point(position)) {
                    if (last_region == regions[i]->ID) {
                        int result = i + direction;
                        if (i + direction == -1) result = 7;
                        else if (i + direction == 8) result = 0;
                        return result;
                    }
                }
                if (regions[i]->in_region(position)) return i;
            }
            return -1;
        }


        /// @brief Retrieves the critical point for a region
        /// @param region The ID of the region
        /// @param IS_UPPER Whether the critical point must be an upper or lower point
        /// @return A pair of coordinates
        std::pair<double, double> get_critical_point(int region, bool IS_UPPER) {
            if (IS_UPPER) return regions[region]->upper_critical_point;
            else return regions[region]->lower_critical_point;
        }

        // coordinates-angle-readjustment - Wa did this it's so cool!
        double x_adjust = -70.20;    // inches
        double y_adjust = -70.20;    // inches
        double headingAdjust = 90;  // degree

        double* triBallInteractionAngle = &(tm->odometry_heading);

        // for angles
        double* TWO_SEVENTY = new double(270 + headingAdjust);
        double* ONE_EIGHTY = new double(180 + headingAdjust);
        double* ONE_THIRTY_FIVE = new double (135 + headingAdjust);
        double* NINETY = new double (90 + headingAdjust);
        double* FORTY_FIVE = new double(45 + headingAdjust);

        double* NEG_ONE_THIRTY_FIVE = new double(-135 + headingAdjust);
        double* NEG_FORTY_FIVE = new double(-45 + headingAdjust);

        double* ZERO = new double(0 + headingAdjust);

        // Map Elements
        std::vector<GameElement*> mapElements;
        std::vector<Buffer*> buffers;
        std::vector<Region*> regions;
        int next_region_id = 0;

};