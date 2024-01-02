#include "PID.h"

PID::PID(RobotConfig *robotConfig, Telemetry *telemetry) {
    rc = robotConfig;
    tm = telemetry;

    current_position = tm->get_current_position();
    last_position = current_position;
}

void PID::correct_drive( //TODO: See if I can get the voltage from the motors instead of manually tracking it
    double left_motors_voltage,
    vex::directionType left_direction,
    double right_motors_voltage,
    vex::directionType right_direction
) {

    double current_heading = tm->get_current_heading();
    double distance_traveled = tm->get_distance_between_points(last_position, current_position);
    cross_track_error += (distance_traveled * sin(get_intended_heading() - current_heading));
    double angle_adjustment = correct_heading() * rc->DRIVETRAINWIDTH; //TODO: May need to multiply by 7
    if (angle_adjustment > 0.0) {
        left_motors_voltage -= angle_adjustment;
        right_motors_voltage += angle_adjustment;
    } else {
        left_motors_voltage += angle_adjustment;
        right_motors_voltage -= angle_adjustment;
    }
    hw->left_drivetrain_motors.spin(left_direction, left_motors_voltage,
                                    vex::voltageUnits::volt);
    hw->right_drivetrain_motors.spin(right_direction, right_motors_voltage,
                                     vex::voltageUnits::volt);


    last_position = current_position;
}

double PID::correct_heading() {

    proportional = cross_track_error;
    derivate = (cross_track_error - previous_cross_track_error) / (1);
    integral += cross_track_error * (1);
    const double proportional_coefficient = 0.000; // TODO: Change to 1.250
    const double derivative_coefficient = 0.000; // TODO: Change to 0.200
    const double integral_coefficient = 0.000; // TODO: Increase to 1.000, see what happens

    double correction = (proportional_coefficient * proportional) +
                        (derivative_coefficient * derivate) +
                        (integral_coefficient * integral);
    
    return correction;
    
}