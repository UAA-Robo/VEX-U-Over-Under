#pragma once
#include <vector>
#include <string>

#include "RobotConfig.h"
#include "Buffer.h"

/*
    Representation of Map
*/



class Map {
    private:

        RobotConfig rc;

        void add_buffer(
        std::pair<double, double> upper_left_corner,
        std::pair<double, double> bottom_right_corner) {
            buffers.push_back(Buffer(upper_left_corner, bottom_right_corner));
        }



    public:
        Map(RobotConfig *robot_config) {
            rc = robot_config;
            double drive_train_adjustment = rc.DRIVETRAINWIDTH * sqrt(2) / 2;
            nextElementID = 0;

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



        // coordinates-angle-readjustment - Wa did this it's so cool!
        double xAdjust = -70.20;    // inches
        double yAdjust = -70.20;    // inches
        double headingAdjust = 90;  // degree

        std::vector<Buffer> buffers;
        int nextElementID;

}