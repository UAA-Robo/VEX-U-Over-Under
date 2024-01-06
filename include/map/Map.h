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
            Buffer bf(upper_left_corner, bottom_right_corner);
            buffers.push_back(&bf);
        }

        void add_simple_region(
        std::pair<double, double> upper_left_corner,
        std::pair<double, double> bottom_right_corner,
        std::pair<double, double> upper_critical_point,
        std::pair<double, double> lower_critical_point
        ) {
            // next_region_id++;
            // std::cout << next_region_id << '\n';
            SimpleRegion* sr = new SimpleRegion( // Yo props to Wa for the ++ thing, very clever
                next_region_id++, upper_left_corner, bottom_right_corner,
                upper_critical_point, lower_critical_point);
            regions.push_back(sr);
            // ++next_region_id;
        }

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

    public:
        Map(RobotConfig *robotConfig) {
            rc = robotConfig;
            double drive_train_adjustment = rc->DRIVETRAINWIDTH * sqrt(2) / 2;

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
            // std::cout << regions.size() << std::endl;
            for (int i = 0; i < regions.size(); ++i) {
                // std::cout << i << " ID" << regions[i]->getID() << std::endl;
                if (regions[i]->is_critical_point(position)) {
                    if (last_region == regions[i]->ID) {
                        // std::cout << "same" << '\n';
                        int result = i + direction;
                        if (i + direction == -1) result = 7;
                        else if (i + direction == 8) result = 0;
                        return result;
                    }
                }
                if (regions[i]->in_region(position)) return i;
            }
            std::cout << "HEllo!" << std::endl;
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

        std::vector<Buffer*> buffers;
        std::vector<Region*> regions;
        int next_region_id = 0;

};