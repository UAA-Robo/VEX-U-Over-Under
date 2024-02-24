#include <cmath>
#include "Drive.h"

Drive::Drive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry)
{
    hw = hardware;
    rc = robotConfig;
    tm = telemetry;

    //setup_bot();
    // pid = new PID(rc, tm);
}

std::pair<double, double> Drive::calculate_drivetrain_velocity(std::pair<double, double> velocity_percent)
{
    double vertical_velocity_percent = velocity_percent.first / 100;
    double horizontal_velocity_percent = velocity_percent.second / 110; // To reduce sensitivity when turning 
    if (rc-> ROBOT == SCRAT) {
         horizontal_velocity_percent = velocity_percent.second / 110; // To reduce sensitivity when turning
    }

    // Calculate raw left and right motor velocity
    double raw_left_velocity = vertical_velocity_percent + horizontal_velocity_percent;
    double raw_right_velocity = vertical_velocity_percent - horizontal_velocity_percent;

    // Normalize the motor velocity
    double max_raw_velocity = std::max(std::abs(raw_left_velocity), std::abs(raw_right_velocity));
    double normalization_factor = max_raw_velocity > 1.0 ? max_raw_velocity : 1.0;

    double left_velocity_multiplier = raw_left_velocity / normalization_factor;
    double right_velocity_multiplier = raw_right_velocity / normalization_factor;

    // Multiplying by 100 breaks it?? Why?
    double left_velocity = 95 * left_velocity_multiplier;
    double right_velocity = 95 * right_velocity_multiplier;

    return {left_velocity, right_velocity};
}

void Drive::move_drivetrain(std::pair<double, double> velocity_percent)
{
    std::pair<double, double> velocity = calculate_drivetrain_velocity(velocity_percent);

    hw->left_drivetrain_motors.spin(vex::directionType::fwd, velocity.first, 
                                    vex::velocityUnits::pct);
    hw->right_drivetrain_motors.spin(vex::directionType::fwd, velocity.second, 
                                    vex::velocityUnits::pct);
    
    // if (rc->USE_PID_IN_USERDRIVE) pid->correct_drive();
}

void Drive::activate_intake()
{
    if (rc->ROBOT == SCRAT) hw->intake.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
    else hw->intake.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
    hw->controller.Screen.setCursor(1, 1);
    hw->controller.Screen.print("Activating Intake!");
}

void Drive::reverse_intake()
{
    if (rc->ROBOT == SCRAT) hw->intake.spin(vex::directionType::fwd, 12.0, vex::voltageUnits::volt);
    else hw->intake.spin(vex::directionType::fwd, 12.0, vex::voltageUnits::volt);
    hw->controller.Screen.setCursor(1, 1);
    hw->controller.Screen.print("Reversing Intake!");
}


void Drive::stop_intake()
{
    hw->intake.stop();
}


void Drive::expand_intake()
{
    hw->intake_expansion.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
    hw->controller.Screen.setCursor(1, 1);
    hw->controller.Screen.print("Expanding Intake!");
}


void Drive::retract_intake()
{
    hw->intake_expansion.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
    hw->controller.Screen.setCursor(1, 1);
    hw->controller.Screen.print("Retracting Intake!");
}


void Drive::stop_intake_expansion()
{
    //hw->intake_expansion.stop(vex::brakeType::coast);
    hw->intake_expansion.stop(vex::brakeType::brake);
}

void Drive::snowplow_out() {
    if (snowplow_timeout > 20) {
        hw->right_plow.set(true);
        hw->left_plow.set(true);
        snowplow_timeout = 0;
    }
}

void Drive::right_snowplow_out() {
    hw->right_plow.set(true);

}

void Drive::left_snowplow_out() {
    hw->left_plow.set(true);

}

void Drive::snowplow_in() {
    if (snowplow_timeout > 20) {
        hw->right_plow.set(false);
        hw->left_plow.set(false);
        snowplow_timeout = 0;
    }

}

void Drive::right_snowplow_in() {
    hw->right_plow.set(false);
}

void Drive::left_snowplow_in() {
    hw->left_plow.set(false);
}

int Drive::run_catapult_thread(void* param)
{
    // WARNING: DON'T print in this thread or it will take too long and miss the catapult press
    Drive* dr = static_cast<Drive*>(param);
    dr->hw->catapult_sensor.resetPosition();
    

    // 0 deg is really 358-2 deg
    double MAX_ANGLE = dr->rc->MAX_CATAPULT_ANGLE - 5; // To slow down early
    double angle; 
    while(true) {
        // STOP catapult
        angle = dr->hw->catapult_sensor.angle(vex::deg);
        if (dr->CATAPULT_RELEASED) {
            dr->hw->catapult.stop(vex::brakeType::coast);
        } else if (angle >= MAX_ANGLE && angle <= 350 && !dr->START_CATAPULT_LAUNCH) {
            // Robots need downward force to stop catapult
            dr->hw->catapult.stop();
            dr->hw->catapult.spin(vex::directionType::rev, 1, vex::voltageUnits::volt);
            
        } else  {
            if (dr->rc->ROBOT == SCRATETTE) {
                dr->hw->catapult.spin(vex::directionType::rev, 10.0, vex::voltageUnits::volt);
            } else {
                dr->hw->catapult.spin(vex::directionType::rev, 12.0, vex::voltageUnits::volt);
            }
        }
        
        vex::wait(10, vex::timeUnits::msec);
        //dr->CATAPULT_RELEASED = false;
    }
}

void Drive::start_catapult() {
    START_CATAPULT_LAUNCH = true;
}

void Drive::stop_catapult() {
    START_CATAPULT_LAUNCH = false;
}

void Drive::release_catapult() {
    CATAPULT_RELEASED = true;
}

void Drive::engage_catapult() {
    CATAPULT_RELEASED = false;
}


void Drive::run_catapult_strategy(int number_triballs, bool TURN) {
    double velocity = 50;
    if (rc->ROBOT == SCRATETTE) velocity = 50;

    // Expand intake
    expand_intake();
    vex::wait(1000, vex::timeUnits::msec);

    stop_intake_expansion();

    // Start Catapult thread
    vex::task catapult_task = vex::task(run_catapult_thread, this, 1);


    // Move + Launch
    for (int i = 0; i < number_triballs - 1; i++) {
        run_catapult_arc_once(false, TURN);
    }

    // Launches last triball and finishes outward
    run_catapult_arc_once(true, TURN);

    stop_intake();
    retract_intake();
    if (rc->ROBOT == SCRATETTE) vex::wait(1000, vex::timeUnits::msec);
    else vex::wait(500, vex::timeUnits::msec);
    stop_intake_expansion();

}

void Drive::run_catapult_once() {
    double velocity = 50;
    if (rc->ROBOT == SCRATETTE) velocity = 80;

    // start intake
    activate_intake();

    // Move + Launch
    turbo_drive_distance(5.5, true, velocity);

    start_catapult();
    if (rc->ROBOT == SCRATETTE) vex::wait(200, vex::timeUnits::msec);
    else vex::wait(500, vex::timeUnits::msec);
    stop_catapult();

    if (rc->ROBOT == SCRATETTE) vex::wait(800, vex::timeUnits::msec);

    turbo_drive_distance(5.6, false, velocity);

}

void Drive::run_catapult_arc_once(bool FINISH_OUTWARD, bool TURN) {
    double drive_velocity = 50;
    double turn_velocity = 30;
    double outward_distance = 4.5;
    double inward_disntance = 5.2;
    if (rc->ROBOT == SCRATETTE)  {
        drive_velocity = 50;
        turn_velocity = 40;
        outward_distance = 5.5;
        inward_disntance = 8;
    }

    // start intake
    activate_intake();

    // Move + Launch
    turbo_drive_distance(outward_distance, true, drive_velocity); 

    
    if (TURN)  {
        if (rc->ROBOT == SCRAT) {
             turbo_turn_relative(330, turn_velocity);

        } else  turbo_turn_relative(325, turn_velocity); // Will go shortest distance so actually -35
    }
    start_catapult();
    vex::wait(100, vex::timeUnits::msec);
    stop_catapult();

    if (TURN)  {
        if (rc->ROBOT == SCRAT) {
             turbo_turn_relative(30, turn_velocity);

        } else  turbo_turn_relative(35, turn_velocity); 
    }

    if (!FINISH_OUTWARD) turbo_drive_distance(inward_disntance, false, drive_velocity);

}

void Drive::turbo_drive_distance(double distance, bool IS_REVERSE, double velocity) {

    double num_wheel_revolutions = distance / rc->WHEEL_CIRCUMFERENCE 
        * rc->DRIVETRAIN_GEAR_RATIO_MULTIPLIER;
    
    // std::pair<double, double> vel = calculateDriveTrainVel(velPercent);

    hw->drivetrain.resetPosition();

    if (IS_REVERSE) num_wheel_revolutions = -num_wheel_revolutions;
    hw->drivetrain.spinTo(num_wheel_revolutions, vex::rotationUnits::rev, velocity, 
        vex::velocityUnits::pct, false);
    
    vex::wait(50, vex::timeUnits::msec);
    //Blocks other tasks from starting 
    while(fabs(hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0.0 
    || fabs(hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0.0){
            vex::wait(20, vex::timeUnits::msec); // To let other threads run
    } 

}

void Drive::turbo_turn(double heading, double velocity)
{
    // Corrects heading to be from 0-360 from the x axis counterclockwise if applicable
    heading = fmod(heading, 360);
    if (heading < 0)
        heading += 360;

    double angle_to_rotate = heading - tm->get_current_heading();
    angle_to_rotate = fmod(angle_to_rotate, 360); // make sure the angle to rotate is -360 to 360
    turbo_turn_relative(angle_to_rotate, velocity);
}

void Drive::turbo_turn_relative(double relative_angle, double velocity) {
    // Determines whether to rotate left or right based on the  shortest distance
    if (360 - fabs(relative_angle) < relative_angle) relative_angle = relative_angle - 360;
    //if (relative_angle > 180) 
    double revolutions = relative_angle  * (rc->DRIVETRAIN_WIDTH) * M_PI 
        / (360 * rc->WHEEL_CIRCUMFERENCE) * rc->DRIVETRAIN_GEAR_RATIO_MULTIPLIER;

    hw->left_drivetrain_motors.resetPosition();
    hw->right_drivetrain_motors.resetPosition();

    hw->left_drivetrain_motors.spinFor(-revolutions, vex::rotationUnits::rev, velocity, 
        vex::velocityUnits::pct, false);
    hw->right_drivetrain_motors.spinFor(revolutions, vex::rotationUnits::rev, velocity, 
        vex::velocityUnits::pct);

    // Blocks other tasks from starting
    while (fabs(hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0 
        || fabs(hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0); 

}

void Drive::arc_in_place(double distance, double min_velocity, double max_velocity, bool IS_CLOCKWISE, bool IS_BACKWARD) {


    const double stopping_aggression = 0.5; //0.1; // Lower number is higher aggression (steeper slope)
    double velocity = min_velocity;

    double num_wheel_revolutions = distance / rc->WHEEL_CIRCUMFERENCE 
        * rc->DRIVETRAIN_GEAR_RATIO_MULTIPLIER;;
    if (IS_BACKWARD) num_wheel_revolutions = -num_wheel_revolutions;
    
    hw->drivetrain.resetPosition();

    vex::motor_group drivetrain_side = hw->right_drivetrain_motors;
    if (IS_CLOCKWISE) {
        drivetrain_side = hw->left_drivetrain_motors;
    }
    drivetrain_side.spinTo(num_wheel_revolutions, vex::rotationUnits::rev, velocity, 
            vex::velocityUnits::pct, false);

    vex::wait(30, vex::timeUnits::msec);

    float current_distance = fabs(drivetrain_side.position(vex::rotationUnits::rev)) 
        * rc->WHEEL_CIRCUMFERENCE;

    // Speeds up as it gets leaves, slows down as it gets closer
    while(distance - current_distance > 0.5) {
        if (current_distance >= distance/2) {
            // First half of distance
            velocity = atan(distance - current_distance) * 2 * (max_velocity-min_velocity) / M_PI 
                + min_velocity;
        } else {
            // Second half of distance
            velocity = atan(stopping_aggression * current_distance) * 2 * max_velocity / M_PI;
        }

        drivetrain_side.setVelocity(velocity, vex::velocityUnits::pct);

        vex::wait(50, vex::timeUnits::msec);
        current_distance = fabs(drivetrain_side.position(vex::rotationUnits::rev)) 
            * rc->WHEEL_CIRCUMFERENCE;
        //std::cout << current_distance << std::endl;
    }

};



void Drive::arc(double distance, double min_velocity, double max_velocity, double velocity_side_difference, bool IS_CLOCKWISE, bool IS_BACKWARD) {
    
    double num_wheel_revolutions = distance / rc->WHEEL_CIRCUMFERENCE
        * rc->DRIVETRAIN_GEAR_RATIO_MULTIPLIER;;
    double stopping_aggression = 0.1;

    vex::directionType direction = vex::directionType::fwd;
    
    if (IS_BACKWARD) {
        num_wheel_revolutions = -num_wheel_revolutions ;
        direction = vex::directionType::rev;
    }

    double faster_velocity = min_velocity;
    
    hw->drivetrain.resetPosition();

    vex::motor_group faster_motors = hw->left_drivetrain_motors;
    vex::motor_group slower_motors = hw->right_drivetrain_motors;

    if (!IS_CLOCKWISE) {
        faster_motors = hw->right_drivetrain_motors;
        slower_motors = hw->left_drivetrain_motors;
    }

    faster_motors.spinTo(num_wheel_revolutions, vex::rotationUnits::rev, 
        faster_velocity, vex::velocityUnits::pct, false);
    slower_motors.spin(direction, faster_velocity * velocity_side_difference, vex::velocityUnits::pct);

    vex::wait(30, vex::timeUnits::msec);

    float current_distance = fabs(faster_motors.position(vex::rotationUnits::rev)) 
        * rc->WHEEL_CIRCUMFERENCE;

    // Speeds up as it gets leaves, slows down as it gets closer
    while(distance - current_distance > 0.5) {
        if (current_distance >= distance/2) {
            // First half of distance
            faster_velocity = atan(distance - current_distance) * 2 * (max_velocity-min_velocity) / M_PI 
                + min_velocity;
        } else {
            // Second half of distance
            faster_velocity = atan(stopping_aggression * current_distance) * 2 * max_velocity / M_PI;
        }

        faster_motors.setVelocity(faster_velocity, vex::velocityUnits::pct);
        slower_motors.setVelocity(faster_velocity * velocity_side_difference, vex::velocityUnits::pct);

        vex::wait(50, vex::timeUnits::msec);
        current_distance = fabs(faster_motors.position(vex::rotationUnits::rev)) 
            * rc->WHEEL_CIRCUMFERENCE;
        //std::cout << current_distance << std::endl;
    }


    slower_motors.stop();

}

