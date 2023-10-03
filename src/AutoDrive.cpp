#include "AutoDrive.h"
#include "Enums.h"
#include "Constants.h"
#include "Node.h"
#include "Graph.h"

#include <iostream>
#include <time.h>
#include <random>
#include <stdexcept>
#include <cmath>
#include "Node.h"
#include "Graph.h"
#include "Constants.h"
#include "Enums.h"
// #include <windows.h>
#include <unistd.h>


#include "vex.h"
#include "Robot.h"

AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) {}

void AutoDrive::drive() {
    test_pathfinding();
}

void AutoDrive::rotate_to_relative_angle(double angle) // Based on ENCODERS,
{

    double number_drivetrain_revolutions = angle * (rc->DRIVETRAINWIDTH) * M_PI / (360 * rc->WHEELCIRC);
    double revolutions_left_wheels = -number_drivetrain_revolutions;
    double revolutions_right_wheels = number_drivetrain_revolutions;

    std::pair<double, double> vel = calculate_drivetrain_velocity({0, rc->auto_rotate_velocity_percent});

    hw->left_drivetrain_motors.spinFor(revolutions_left_wheels, vex::rotationUnits::rev, vel.first, vex::velocityUnits::pct, false);
    hw->right_drivetrain_motors.spinFor(revolutions_right_wheels, vex::rotationUnits::rev, vel.second, vex::velocityUnits::pct);
    while (fabs(hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0 || fabs(hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0)
        ; // Blocks other tasks from starting
}

void AutoDrive::rotate_to_heading(double heading)
{
    // Corrects heading to be from 0-360 from the x axis counterclockwise if applicable
    heading = fmod(heading, 360);
    if (heading < 0)
        heading += 360;

    {
        double angle_to_rotate = heading - tm->get_current_heading();
        angle_to_rotate = fmod(angle_to_rotate, 360); // make sure the angle to rotate is -360 to 360

        // Determines whether to rotate left or right based on the  shortest distance
        if (360 - fabs(angle_to_rotate) < angle_to_rotate)
            angle_to_rotate = angle_to_rotate - 360;
        rotate_to_relative_angle(angle_to_rotate + robot_angle_offset);
    }
    tm->set_current_heading(heading);
    // tm->headingErrorCorrection();
    // tm->positionErrorCorrection();
}

void AutoDrive::rotate_to_position(std::pair<double, double> final_position, bool ISBACKROTATION)
{
    // if (IS_USING_GPS_POSITION) tm->set_current_heading(tm->getGPSPosition());
    double heading = tm->get_heading_between_points(tm->get_current_position(), final_position);

    if (ISBACKROTATION)
        heading -= 180;
    rotate_to_heading(heading);
}

void AutoDrive::rotate_and_drive_to_position(std::pair<double, double> position, bool ISBACKTOPOSITION)
{

    rotate_to_position(position, ISBACKTOPOSITION);

    // if (IS_USING_GPS_HEADING) tm->set_current_heading(tm->getGPSPosition());

    double distance_to_position = tm->get_distance_between_points(tm->get_current_position(), position); // inches
    if (ISBACKTOPOSITION)
        distance_to_position = -distance_to_position;
    move_drivetrain_distance({rc->auto_drive_velocity_percent, 0}, distance_to_position); // Drive at auto_drive_velocity_percent% velocity
}


void AutoDrive::test_pathfinding() {
    std::cout << "ZZZZZZZZZZZZZZZZ\n";
    
    Graph *graph = new Graph(X_NODES_COUNT, Y_NODES_COUNT, NODE_SIZE);
    // std::vector<Node*> path = new std::vector<Node*>;
    std::cout << "ZZZZZZZZZZZZZZZZ\n";
    std::vector<Node*> *path;
    std::cout << "ZZZZZZZZZZZZZZZZ\n";

    srand(time(0));

    std::mt19937 rng(rand());
    std::uniform_int_distribution<int> randX(0, X_NODES_COUNT - 1);
    std::uniform_int_distribution<int> randY(0, Y_NODES_COUNT - 1);

    int originX;
    int originY;
    int destinationX;
    int destinationY;

    
    

    while(true) {
        std::pair<double, double> origin = tm->get_current_position();
        originX = origin.first;
        originY = origin.second; 

        do
        {
            destinationX = randX(rng);
            destinationY = randY(rng);
        } while (graph->get_node(originX, originY)->get_is_forbidden() || graph->get_node(destinationX, destinationY)->get_is_forbidden() || (originX == destinationX && originY == destinationY));

        path = graph->get_path(graph->get_node(originX, originY), graph->get_node(destinationX, destinationY));

        for(int i = 0; i < path->size() - 1; i++) {
            Node* current_node = path->at(i);
            Node* next_node = path->at(i + 1);
            std::pair<double, double> current_point;
            std::pair<double, double> next_point;
            current_point.first = double(current_node->x);
            current_point.second = double(current_node->y);
            next_point.first = double(next_node->x);
            next_point.second = double(next_node->y);
            std::pair<double, double> current_point_gps;
            std::pair<double, double> next_point_gps;

            current_point_gps.first = current_point.second - 69;
            current_point_gps.second = current_point.first - 69;
            next_point_gps.first = next_point.second - 69;
            next_point_gps.second = next_point.first - 69;


            double x1 = current_point.first;
            double y1 = current_point.second;
            double x2 = next_point.first;
            double y2 = next_point.second;
            // return std::sqrt(std::pow(x2 - x1, 2) * 10 + std::pow(y2 - y1, 2) * 10);
            double distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
            
            // rotate_to_position(next_point_gps);
            // void AutoDrive::rotate_and_drive_to_position(std::pair<double, double> position, bool ISBACKTOPOSITION)
            rotate_and_drive_to_position(next_point_gps);
        }

        // sleep(2);
        vex::wait(2000, vex::msec);
    }
    
}
