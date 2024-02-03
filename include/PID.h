#pragma once
#include <utility>
#include "Telemetry.h"
#include "RobotConfig.h"
#include "Hardware.h"

class PID {
    public:
        PID(RobotConfig *robotConfig, Hardware *hardware, Telemetry *telemetry);

        void correct_linear_drive(
            double left_motors_percent,
            double right_motors_percent
        );

        void correct_turn_drive(
            double left_motors_percent,
            vex::directionType left_direction,
            double right_motors_percent,
            vex::directionType right_direction
        );



        void reset_linear_drive(std::pair<double, double> source_point, double heading);

        void reset_turn_drive(double change_heading);

        double get_intended_heading() { return path_heading; }

        std::pair<double, double> get_source_position() { return path_source_position; }

    private:
        RobotConfig *rc;
        Hardware *hw;
        Telemetry *tm;

        double cross_track_error;
        std::pair<double, double> current_position;
        std::pair<double, double> last_position;

        double proportional = 0.0, derivate = 0.0, integral = 0.0;
        double previous_cross_track_error;
        std::pair<double, double> path_source_position;
        double path_heading;

        double correct_heading();
};