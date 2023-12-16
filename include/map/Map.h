#pragma once
#include <vector>
#include <string>

#include "RobotConfig.h"
#include "Buffer.h"
#include "Region.h"

/*
    Representation of Map
*/



class Map {
    private:

        RobotConfig *rc;

        void add_buffer(
        std::pair<double, double> upper_left_corner,
        std::pair<double, double> bottom_right_corner) {
            buffers.push_back(Buffer(upper_left_corner, bottom_right_corner));
        }

        void add_simple_region(
        std::pair<double, double> upper_left_corner,
        std::pair<double, double> bottom_right_corner,
        std::pair<double, double> upper_critical_point,
        std::pair<double, double> lower_critical_point
        ) {
            regions.push_back(SimpleRegion( // Yo props to Wa for the ++ thing, very clever
                next_region_id++, upper_left_corner, bottom_right_corner,
                upper_critical_point, lower_critical_point));
        }

        void add_composite_region(
        std::pair<double, double> upper_left_corner_1,
        std::pair<double, double> bottom_right_corner_1,
        std::pair<double, double> upper_left_corner_2,
        std::pair<double, double> bottom_right_corner_2,
        std::pair<double, double> upper_critical_point,
        std::pair<double, double> lower_critical_point
        ) {
            regions.push_back(CompositeRegion(
                next_region_id++, upper_left_corner_1, bottom_right_corner_1,
                upper_left_corner_2, bottom_right_corner_2,
                upper_critical_point, lower_critical_point));
        }

    public:
        Map(RobotConfig *robotConfig) {
            rc = robotConfig;
            double drive_train_adjustment = rc->DRIVETRAINWIDTH * sqrt(2) / 2;

            /*
            ---- REGIONS ---------------------------------------------------------------------------
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
                {0.0 + xAdjust, 46.64 + yAdjust},
                {23.08 + xAdjust, 0.0 + yAdjust},
                {22.89 + xAdjust, 23.08 + yAdjust},
                {46.83 + xAdjust, 0.0 + yAdjust},
                {34.955 + xAdjust, 34.86 + yAdjust},
                {34.955 + xAdjust, 11.445 + yAdjust}
            );
            add_simple_region( // Left Offensive Region
                {22.89 + xAdjust, 117.52 + yAdjust},
                {70.20 + xAdjust, 22.89 + yAdjust},
                {34.955 + xAdjust, 105.55 + yAdjust},
                {34.955 + xAdjust, 34.86 + yAdjust}
            );
            add_composite_region( // Top Left L-Region
                {0.0 + xAdjust, 140.41 + yAdjust},
                {23.08 + xAdjust, 93.77 + yAdjust},
                {22.89 + xAdjust, 140.41 + yAdjust},
                {46.83 + xAdjust, 117.33 + yAdjust},
                {34.955 + xAdjust, 128.965 + yAdjust},
                {34.955 + xAdjust, 105.55 + yAdjust}   
            );
            add_simple_region( // Top Way Region
                {46.83 + xAdjust, 140.41 + yAdjust},
                {93.58 + xAdjust, 117.33 + yAdjust},
                {105.55 + xAdjust, 128.965 + yAdjust},
                {34.955 + xAdjust, 128.965 + yAdjust}
            );
            add_composite_region( // Top Right L-Region
                {93.58 + xAdjust, 140.41 + yAdjust},
                {117.52 + xAdjust, 117.33 + yAdjust},
                {117.33 + xAdjust, 140.41 + yAdjust},
                {140.41 + xAdjust, 93.77 + yAdjust},
                {105.55 + xAdjust, 105.645 + yAdjust},
                {105.55 + xAdjust, 128.965 + yAdjust}
            );
            add_simple_region( // Right Offensive Region
                {70.20 + xAdjust, 117.52 + yAdjust},
                {117.52 + xAdjust, 22.89 + yAdjust},
                {105.55 + xAdjust, 34.86 + yAdjust},
                {105.55 + xAdjust, 105.645 + yAdjust}
            );
            add_composite_region( // Lower Right L-Region
                {93.58 + xAdjust, 23.08 + yAdjust},
                {117.52 + xAdjust, 0.0 + yAdjust},
                {117.33 + xAdjust, 46.64 + yAdjust},
                {140.41 + xAdjust, 0.0 + yAdjust},
                {105.55 + xAdjust, 11.445 + yAdjust},
                {105.55 + xAdjust, 34.86 + yAdjust}
            );
            add_simple_region( // Bottom Way Region
                {46.83 + xAdjust, 23.08 + yAdjust},
                {93.58 + xAdjust, 0.0 + yAdjust},
                {34.955 + xAdjust, 11.445 + yAdjust},
                {105.55 + xAdjust, 11.445 + yAdjust}
            );

            /*
            ---- BUFFERS ---------------------------------------------------------------------------
            */

            add_buffer( // Blue Goal
                { // Upper Left Corner
                    0.0 - drive_train_adjustment + xAdjust,
                    97.77 + drive_train_adjustment + yAdjust
                },
                { // Bottom Right Corner
                    27.08 + drive_train_adjustment + xAdjust,
                    42.64 - drive_train_adjustment + yAdjust
                }
            );
            add_buffer( // Red Goal
                { // Upper Left Corner
                    113.33 - drive_train_adjustment + xAdjust,
                    97.77 + drive_train_adjustment + yAdjust
                },
                { // Bottom Right Corner
                    140.41 + drive_train_adjustment + xAdjust,
                    42.64 - drive_train_adjustment + yAdjust
                }
            );
            add_buffer( // Bottom Barrier Bar
                { // Upper Left Corner
                    45.83 - drive_train_adjustment + xAdjust,
                    24.58 + drive_train_adjustment + yAdjust
                },
                { // Bottom Right Corner
                    94.58 + drive_train_adjustment + xAdjust,
                    21.39 - drive_train_adjustment + yAdjust
                }
            );
            add_buffer( // Top Barrier Bar
                { // Upper Left Corner
                    45.83 - drive_train_adjustment + xAdjust,
                    119.02 + drive_train_adjustment + yAdjust
                },
                { // Bottom Right Corner
                    94.58 + drive_train_adjustment + xAdjust,
                    115.83 - drive_train_adjustment +yAdjust
                }
            );
            add_buffer( // Middle Barrier Bar
                { // Upper Left Corner
                    68.7 - drive_train_adjustment + xAdjust,
                    119,02 + drive_train_adjustment + yAdjust
                },
                { // Bottom Right Corner
                    71.7 + drive_train_adjustment + xAdjust,
                    21.39 - drive_train_adjustment + yAdjust
                }
            );
        }

        int in_which_region(std::pair<double, double> position) {
            for (int i = 0; i < regions.size(); ++i) {
                if (regions[i].in_region(position)) return i;
            }
            return -1;
        }

        // coordinates-angle-readjustment - Wa did this it's so cool!
        double xAdjust = -70.20;    // inches
        double yAdjust = -70.20;    // inches
        double headingAdjust = 90;  // degree

        std::vector<Buffer> buffers;
        std::vector<Region> regions;
        int next_region_id;

};