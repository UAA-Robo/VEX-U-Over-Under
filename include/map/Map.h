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
        int next_element_id;
        RobotConfig *rc;
        Telemetry* tm;
        bool IS_SKILLS;

        /// @brief Adds a buffer to the map.
        /// @param upper_left_corner The upper left corner coordinates.
        /// @param bottom_right_corner The bottom right corner coordinates.
        void add_buffer(
        std::pair<double, double> upper_left_corner,
        std::pair<double, double> bottom_right_corner) {
            Buffer* bf = new Buffer(upper_left_corner, bottom_right_corner);
            buffers.push_back(bf);
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

        void add_wall(double upper_left_x, double upper_left_y, double lower_right_x, double lower_right_y) {
            walls.push_back(new Obstacle(next_element_id++, upper_left_x, upper_left_y, lower_right_x, lower_right_y));
        }

        void add_startzone(double upper_left_x, double upper_left_y, double lower_right_x, double lower_right_y) {
            startzones.push_back(new Obstacle(next_element_id++, upper_left_x, upper_left_y, lower_right_x, lower_right_y));
        }

        void add_triball(double x, double y, char team_color){
            triballs.push_back(new InteractionObject(next_element_id++, x, y, team_color, triball_interaction_angle, true));
        }

        void add_loadzone(double x, double y, char team_color, double* interaction_angle) {
            loadzones.push_back(new InteractionObject(next_element_id++, x, y, team_color, interaction_angle));
        }

        void add_goal(double x, double y, char team_color, double* interaction_angle) {
            goals.push_back(new InteractionObject(next_element_id++, x, y, team_color, interaction_angle));
        }

        void add_bar(double x, double y, char team_color, double* interaction_angle) {
            bars.push_back(new InteractionObject(next_element_id++, x, y, team_color, interaction_angle));
        }


    public:
        Map(Telemetry* telemetry, RobotConfig *robotConfig, bool IS_SKILLS) {
            tm = telemetry;
            rc = robotConfig;
            next_element_id = 0;
            this->IS_SKILLS = IS_SKILLS;
            double drive_train_adjustment = rc->ACTUAL_RADIUS;

            // outer walls
            add_wall(0 + x_adjust, 140.40 + y_adjust, 0 + x_adjust, 0 + y_adjust);                  // wall 0                                                  
            add_wall(0 + x_adjust, 140.40 + y_adjust, 140.40 + x_adjust, 140.40 + y_adjust);        // wall 1
            add_wall(0 + x_adjust, 0 + y_adjust, 140.40 + x_adjust, 0 + y_adjust);                  // wall 2
            add_wall(140.40 + x_adjust, 140.40 + y_adjust, 0 + x_adjust, 140.40 + y_adjust);        // wall 3

            // inner walls
            add_wall(45.61 + x_adjust, 118.51 + y_adjust, 94.79 + x_adjust, 118.51 + y_adjust);     // wall 4
            add_wall(45.61 + x_adjust, 118.51 + y_adjust, 94.79 + x_adjust, 118.51 + y_adjust);     // wall 5
            add_wall(69.01 + x_adjust, 118.51 + y_adjust, 69.01 + x_adjust, 21.89 + y_adjust);      // wall 6
            add_wall(71.39 + x_adjust, 118.51 + y_adjust, 71.39 + x_adjust, 21.89 + y_adjust);      // wall 7
            add_wall(45.61 + x_adjust, 24.26 + y_adjust, 94.79 + x_adjust, 24.26 + y_adjust);       // wall 8
            add_wall(45.61 + x_adjust, 21.89 + y_adjust, 94.79 + x_adjust, 21.89 + y_adjust);       // wall 9

            /*
            ----HEAD TO HEAD TRIBALLS --------------------------------------------------------------
            */

            if (!IS_SKILLS)
            {
                // red
                add_triball(46.64 + x_adjust, 70.20 + y_adjust, 'R');                               // triball 0
                add_triball(65.95 + x_adjust, 93.77 + y_adjust, 'R');                               // triball 1
                add_triball(65.95 + x_adjust, 70.20 + y_adjust, 'R');                               // triball 2
                    // nuetral
                add_triball(70.20 + x_adjust, 129.11 + y_adjust, 'N');                              // triball 3
                add_triball(70.20 + x_adjust, 11.30 + y_adjust, 'N');                               // triball 4
                    // blue
                add_triball(74.46 + x_adjust, 70.20 + y_adjust, 'B');                               // triball 5
                add_triball(74.46 + x_adjust, 46.64 + y_adjust, 'B');                               // triball 6
                add_triball(93.77 + x_adjust, 70.20 + y_adjust, 'B');                               // triball 7
            }

            /*
            ----SKILLS TRIBALLS --------------------------------------------------------------------
            */
            else
            {
                    // red
                add_triball(44.64 + x_adjust, 70.22 + y_adjust, 'R');                               // triball 0
                add_triball(60.22 + x_adjust, 113.17 + y_adjust, 'R');                              // triball 1
                add_triball(60.22 + x_adjust, 93.79 + y_adjust, 'R');                               // triball 2
                add_triball(60.22 + x_adjust, 70.22 + y_adjust, 'R');                               // triball 3
                add_triball(60.22 + x_adjust, 46.66 + y_adjust, 'R');                               // triball 4
                add_triball(60.22 + x_adjust, 27.28 + y_adjust, 'R');                               // triball 5

                    // nuetral
                add_triball(70.20 + x_adjust, 129.13 + y_adjust, 'N');                              // triball 6
                add_triball(70.20 + x_adjust, 11.32 + y_adjust, 'N');                               // triball 7
            }

            /*
            ----------------------------------------------------------------------------------------
            */

            // load Zones
            
                // red
            add_loadzone(22.89 / 2 + x_adjust, 129.11 + y_adjust, 'R', FORTY_FIVE);                 // loadzone 0
            add_loadzone(22.89 / 2 + x_adjust, 11.30  + y_adjust, 'R', ONE_THIRTY_FIVE);            // loadzone 1

                // blue
            add_loadzone(122.76 + x_adjust, 129.11 + y_adjust, 'B', NEG_FORTY_FIVE);                // loadzone 2
            add_loadzone(122.76 + x_adjust, 11.30 + y_adjust, 'B', NEG_ONE_THIRTY_FIVE);            // loadzone 3


            // corners-starting zones
            add_startzone(0 + x_adjust, 140.41 + y_adjust, 70.20 + x_adjust, 58.42 + y_adjust);     // startzone 0
            add_startzone(0 + x_adjust, 70.20 + y_adjust, 70.20 + x_adjust, 0 + y_adjust);          // startzone 1
            add_startzone(70.20 + x_adjust, 140.41 + y_adjust, 140.41 + x_adjust, 70.20 + y_adjust);// startzone 2
            add_startzone(70.20 + x_adjust, 81.985 +y_adjust, 140.41 + x_adjust, 0 + y_adjust);     // startzone 3

            // triball goals--interaction point
                // red
            add_goal(117.09 + x_adjust, 81.985 + y_adjust, 'R', ONE_EIGHTY);                          // goal 0
            add_goal(117.09 + x_adjust, 70.20 + y_adjust, 'R', ZERO);                                // goal 1
            add_goal(117.09 + x_adjust, 58.41 + y_adjust, 'R', NINETY);                             // goal 2
            add_goal(128.75 + x_adjust, 93.77 + y_adjust, 'R', NINETY);                              // goal 3
            add_goal(128.75 + x_adjust, 46.64 + y_adjust, 'R', NINETY);                              // goal 4

                //blue
            add_goal(11.66 + x_adjust, 93.77 + y_adjust, 'B', TWO_SEVENTY);                       // goal 5
            add_goal(11.66 + x_adjust, 46.64 + y_adjust, 'B', TWO_SEVENTY);                        // goal 6
            add_goal(23.32 + x_adjust, 81.985 + y_adjust, 'B', TWO_SEVENTY);                        // goal 7
            add_goal(23.32 + x_adjust, 70.20 + y_adjust, 'B', ONE_EIGHTY);                         // goal 8
            add_goal(23.32 + x_adjust, 58.41 + y_adjust, 'B', ZERO);                               // goal 9


        /*
            PLACE HOLDERS!!!                // TODO: Change Later once robot is built
        */
            // HighBar
            add_bar(70.20 + x_adjust, 129.11 + y_adjust, 'B', triball_interaction_angle);           // bar 0
            add_bar(70.20 + x_adjust, 11.30 + y_adjust, 'R', triball_interaction_angle);            // bar 2

            // Elevation Bar
            add_bar(70.20 + x_adjust, 117.33 + y_adjust, 'B', triball_interaction_angle);           // bar 3
            add_bar(70.20 + x_adjust, 23.08 + y_adjust, 'R', triball_interaction_angle);            // bar 4

        


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
                //{34.955 + x_adjust, 34.86 + y_adjust},
                {34.955 + x_adjust, 128.965 + 1 + y_adjust}, // Add one to not get stuck on bar
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

        /// @brief Detects whether a given position falls in a buffer region.
        /// @param position The position to check (x, y).
        /// @return True if in a buffer region, false otherwise.
        bool in_buffer(std::pair<double, double> position) {
            for (int i = 0; i < buffers.size(); ++i) {
                if (buffers.at(i)->in_buffer(position)) {
                    std::cout << "In buffer: " << i << '\n';
                    return true;}
            }
            return false;
        }



        /// @brief Provides the position of interaction for a game element based off its interaction_angle
        /// @param element GameElement passed in to recieve new value
        /// @param CAN_TURN_AROUND Determines if the bot should have enough room to rotate
        /// @return GameElement position with offset
        std::pair<double, double> get_point_with_offset(InteractionObject* element, bool CAN_TURN_AROUND)
        {
            double offset_distance = rc->ACTUAL_WIDTH / 2.0;
            std::pair<double, double> new_position = element->get_position();

            std::cout << "MAP ORIGINAL POS: " << new_position.first << ", " << new_position.second << std::endl;

            // If the robot should be able to rotate, add more offset
            if (CAN_TURN_AROUND) { offset_distance = rc->ACTUAL_WIDTH * sqrt(2) / 2.0 + 1; } // change later

            std::cout << "MAP POINT OFFSET DISTANCE: " << offset_distance << std::endl;
            new_position.first += -1 * offset_distance 
                * cos(element->get_interaction_angle() * M_PI/180);
            new_position.second += -1 * offset_distance 
                * sin(element->get_interaction_angle() * M_PI/180);
            std::cout << "MAP OFFSET POS: " << new_position.first << ", " << new_position.second << std::endl;

            return new_position;
        }

        // coordinates-angle-readjustment - Wa did this it's so cool!
        double x_adjust = -70.20;    // inches
        double y_adjust = -70.20;    // inches
        double heading_adjust = 90;  // degree

        double* triball_interaction_angle = &(tm->odometry_heading);

        // for angles
        double* TWO_SEVENTY = new double(270 + heading_adjust);
        double* ONE_EIGHTY = new double(180 + heading_adjust);
        double* ONE_THIRTY_FIVE = new double (135 + heading_adjust);
        double* NINETY = new double (90 + heading_adjust);
        double* FORTY_FIVE = new double(45 + heading_adjust);

        double* NEG_ONE_THIRTY_FIVE = new double(-135 + heading_adjust);
        double* NEG_FORTY_FIVE = new double(-45 + heading_adjust);

        double* ZERO = new double(0 + heading_adjust);

        // Map Elements
        std::vector<InteractionObject*> loadzones;
        std::vector<Obstacle*> startzones;
        std::vector<InteractionObject*> goals;
        std::vector<InteractionObject*> triballs;
        std::vector<Obstacle*> walls;
        std::vector<InteractionObject*> bars;
        std::vector<Buffer*> buffers;
        std::vector<Region*> regions;
        int next_region_id = 0;

};