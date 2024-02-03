#include "PID.h"

PID::PID(RobotConfig *robotConfig, Hardware *hardware, Telemetry *telemetry) {
    rc = robotConfig;
    hw = hardware;
    tm = telemetry;

    current_position = tm->get_current_position();
    last_position = current_position;
}

void PID::correct_linear_drive(
    double left_motors_voltage,
    double right_motors_voltage
) {
    // double left_motors_voltage = hw->left_drivetrain_motors.voltage(vex::voltageUnits::volt);
    // double right_motors_voltage = hw->right_drivetrain_motors.voltage(vex::voltageUnits::volt);

    double current_heading = tm->get_current_heading();
    double distance_traveled = tm->get_distance_between_points(last_position, current_position);
    cross_track_error += (distance_traveled * sin(get_intended_heading() - current_heading));
    double angle_adjustment = correct_heading() * rc->DRIVETRAIN_WIDTH; //TODO: May need to multiply by 7
    std::cout << "Angle adjust: " << angle_adjustment << '\n';
    if (angle_adjustment > 0.0) {
        left_motors_voltage -= angle_adjustment;
        right_motors_voltage += angle_adjustment;
    } else {
        left_motors_voltage += angle_adjustment;
        right_motors_voltage -= angle_adjustment;
    }
    hw->left_drivetrain_motors.spin(vex::directionType::fwd, left_motors_voltage,
                                    vex::voltageUnits::volt);
    hw->right_drivetrain_motors.spin(vex::directionType::fwd, right_motors_voltage,
                                     vex::voltageUnits::volt);


    last_position = current_position;
}

void PID::reset_linear_drive(std::pair<double, double> source_point, double heading) {

    cross_track_error = 0.0;
    path_source_position = source_point;
    path_heading = heading;
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