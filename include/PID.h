#pragma once
#include <utility>
#include "Telemetry.h"
#include "RobotConfig.h"

class PID {
    public:
        PID(RobotConfig *robotConfig, Telemetry *telemetry);

        void correct_drive(
            double left_motors_voltage,
            vex::directionType left_direction,
            double right_motors_voltage,
            vex::directionType right_direction
        );

        void reset_path(std::pair<double, double> source_point, double heading);

        double get_intended_heading() { return path_heading; }

        std::pair<double, double> get_source_position() { return path_source_position; }

    private:
        RobotConfig *rc;
        Telemetry *tm;

        double cross_track_error;
        std::pair<double, double> current_position;
        std::pair<double, double> last_position;

        double proportional = 0.0, derivate = 0.0, integral = 0.0;
        double previous_cross_track_error;
        std::pair<double, double> path_source_position;
        double path_heading;

        double correct_heading();
}