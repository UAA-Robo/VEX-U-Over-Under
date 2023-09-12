#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
using namespace std;

double dist(double *in1, double *in2) { // Distance formula returns double value
    double out;
    out = sqrt((in2[0] - in1[0])*(in2[0] - in1[0]) + (in2[1] - in1[1])*(in2[1] - in1[1]));
    return out;
}

// Relative Coordinate Function; IN: 2x Coordinate Pointers, OUT: 1x Relative Coordinate Pointer
double* relative(double *in1, double *in2) { // Returns TARGET relative to SOURCE 1x2 Array
    static double out[2] = {0.0, 0.0};
    cout << "CURRENT POS: " << in1[0] << " " << in1[1] << endl;
    out[0] = in2[0] - in1[0];
    out[1] = in2[1] - in1[1];
    return out;
}

double* rec2pol(double *in) {
    static double out[2] = {0.0, 0.0};
    out[0] = sqrt((in[0] * in[0]) +(in[1] * in[1]));
    out[1] = atan(in[1] / in[0]);
    out[1] = (out[1] < 0.0 && in[0] < 0.0) ? 3.1415927 + out[1] : out[1];
    return out;
}

double* pol2rec(double *in) {
    static double out[2] = {0.0, 0.0};
    out[0] = in[0] * cos(in[1]);
    out[1] = in[0] * sin(in[1]);
    //out[0] = (in[1] > 1.570796 && in[1] < 4.71238898) ? -out[0] : out[0];
    return out;
}

int in_zone(double *in) {
    int zone = -1;
    const double ZONE_7[4][2] = {{0.0, 0.0}, {0.0, 50.0}, {150.0, 0.0}, {150.0, 50.0}}; // BLUE left way
    const double ZONE_6[4][2] = {{0.0, 50.0}, {0.0, 150.0}, {150.0, 50.0}, {150.0, 150.0}}; // BLUE left offensive
    const double ZONE_5[4][2] = {{0.0, 150.0}, {0.0, 250.0}, {150.0, 150.0}, {150.0, 250.0}}; // BLUE right offensive
    const double ZONE_4[4][2] = {{0.0, 250.0}, {0.0, 300.0}, {150.0, 250.0}, {150.0, 300.0}}; // BLUE right way
    const double ZONE_0[4][2] = {{150.0, 0.0}, {150.0, 50.0}, {300.0, 0.0}, {300.0, 50.0}}; // RED right way
    const double ZONE_1[4][2] = {{150.0, 50.0}, {150.0, 150.0}, {300.0, 50.0}, {300.0, 150.0}}; // RED right offensive
    const double ZONE_2[4][2] = {{150.0, 150.0}, {150.0, 250.0}, {300.0, 150.0}, {300.0, 250.0}}; // RED left offensive
    const double ZONE_3[4][2] = {{150.0, 250.0}, {150.0, 300.0}, {300.0, 250.0}, {300.0, 300.0}}; // RED left way

    zone = ((in[0] <= ZONE_7[2][0] && in[0] >= ZONE_7[0][0]) && (in[1] >= ZONE_7[0][1] && in[1] <= ZONE_7[1][1])) ? 7 : zone;
    zone = ((in[0] <= ZONE_6[2][0] && in[0] >= ZONE_6[0][0]) && (in[1] >= ZONE_6[0][1] && in[1] <= ZONE_6[1][1])) ? 6 : zone;
    zone = ((in[0] <= ZONE_5[2][0] && in[0] >= ZONE_5[0][0]) && (in[1] >= ZONE_5[0][1] && in[1] <= ZONE_5[1][1])) ? 5 : zone;
    zone = ((in[0] <= ZONE_4[2][0] && in[0] >= ZONE_4[0][0]) && (in[1] >= ZONE_4[0][1] && in[1] <= ZONE_4[1][1])) ? 4 : zone;
    zone = ((in[0] <= ZONE_0[2][0] && in[0] >= ZONE_0[0][0]) && (in[1] >= ZONE_0[0][1] && in[1] <= ZONE_0[1][1])) ? 0 : zone;
    zone = ((in[0] <= ZONE_1[2][0] && in[0] >= ZONE_1[0][0]) && (in[1] >= ZONE_1[0][1] && in[1] <= ZONE_1[1][1])) ? 1 : zone;
    zone = ((in[0] <= ZONE_2[2][0] && in[0] >= ZONE_2[0][0]) && (in[1] >= ZONE_2[0][1] && in[1] <= ZONE_2[1][1])) ? 2 : zone;
    zone = ((in[0] <= ZONE_3[2][0] && in[0] >= ZONE_3[0][0]) && (in[1] >= ZONE_3[0][1] && in[1] <= ZONE_3[1][1])) ? 3 : zone;

    return zone;
}

bool obstructed(double *curr, double *TARGET) {
    bool out = 0;
    const double robot_width = 15 * 2.54;
    double robot_radius = robot_width / 2;
    const double CENTER[] = {150.0, 150.0};
    const double ZERO_WALL_APEX[] = {150.0, 50.0};
    const double ZERO_WALL_BLUE[] = {100.0, 50.0};
    const double ZERO_WALL_RED[] = {200.0, 50.0};
    const double PI_WALL_APEX[] = {150.0, 250.0};
    const double PI_WALL_BLUE[] = {100.0, 250.0};
    const double PI_WALL_RED[] = {200.0, 250.0};
    const double BLUE_GOAL_CORNERS[4][2] = {{0.0, 95.0}, {0.0, 205.0}, {55.0, 95.0}, {55.0, 205.0}};
    const double RED_GOAL_CORNERS[4][2] = {{245.0, 95.0}, {245.0, 205.0}, {300.0, 95.0}, {300.0, 205.0}};

    // Obstruction detection for BARRIER
    if (((curr[0] < CENTER[0] && TARGET[0] > CENTER[0]) || (curr[0] > CENTER[0] && TARGET[0] < CENTER[0]))) out = ((curr[1] < (ZERO_WALL_APEX[1] - robot_radius) && TARGET[1] < (ZERO_WALL_APEX[1] - robot_radius)) || (curr[1] > (PI_WALL_APEX[1] + robot_radius) && TARGET[1] > (PI_WALL_APEX[1] + robot_radius))) ? out : 1;
    if (((curr[1] > ZERO_WALL_APEX[1] && TARGET[1] < ZERO_WALL_APEX[1]) || (curr[1] < ZERO_WALL_APEX[1] && TARGET[1] > ZERO_WALL_APEX[1]))) out = ((curr[0] < (ZERO_WALL_BLUE[0] - robot_radius) && TARGET[0] < (ZERO_WALL_BLUE[0] - robot_radius)) || (curr[0] > (ZERO_WALL_RED[0] + robot_radius) && TARGET[0] > (ZERO_WALL_RED[0] + robot_radius))) ? out : 1;
    if (((curr[1] > PI_WALL_APEX[1] && TARGET[1] < PI_WALL_APEX[1]) || (curr[1] < PI_WALL_APEX[1] && TARGET[1] > PI_WALL_APEX[1]))) out = ((curr[0] < (PI_WALL_BLUE[0] - robot_radius) && TARGET[0] < (PI_WALL_BLUE[0] - robot_radius)) || (curr[0] > (PI_WALL_RED[0] + robot_radius) && TARGET[0] > (PI_WALL_RED[0] + robot_radius))) ? out : 1;
    // Obstruction detection for BLUE GOAL
    if (((curr[0] < BLUE_GOAL_CORNERS[2][0] && TARGET[0] > BLUE_GOAL_CORNERS[2][0]) || (curr[0] > BLUE_GOAL_CORNERS[2][0] && TARGET[0] < BLUE_GOAL_CORNERS[2][0]))) out = ((curr[1] < (BLUE_GOAL_CORNERS[2][1] - robot_radius) && TARGET[1] < (BLUE_GOAL_CORNERS[2][1] - robot_radius)) || (curr[1] > (BLUE_GOAL_CORNERS[3][1] + robot_radius) && TARGET[1] > (BLUE_GOAL_CORNERS[3][1] + robot_radius))) ? out : 1;
    if (((curr[1] > BLUE_GOAL_CORNERS[1][1] && TARGET[1] < BLUE_GOAL_CORNERS[0][1]) || (curr[1] < BLUE_GOAL_CORNERS[0][1] && TARGET[1] > BLUE_GOAL_CORNERS[1][1]))) out = (curr[0] > (BLUE_GOAL_CORNERS[2][0] + robot_radius) && TARGET[0] > (BLUE_GOAL_CORNERS[2][0] + robot_radius)) ? out : 1;
    // Obstruction detection for RED GOAL
    if (((curr[0] < RED_GOAL_CORNERS[2][0] && TARGET[0] > RED_GOAL_CORNERS[2][0]) || (curr[0] > RED_GOAL_CORNERS[2][0] && TARGET[0] < RED_GOAL_CORNERS[2][0]))) out = ((curr[1] < (RED_GOAL_CORNERS[2][1] - robot_radius) && TARGET[1] < (RED_GOAL_CORNERS[2][1] - robot_radius)) || (curr[1] > (RED_GOAL_CORNERS[3][1] + robot_radius) && TARGET[1] > (RED_GOAL_CORNERS[3][1] + robot_radius))) ? out : 1;
    if (((curr[1] > RED_GOAL_CORNERS[1][1] && TARGET[1] < RED_GOAL_CORNERS[0][1]) || (curr[1] < RED_GOAL_CORNERS[0][1] && TARGET[1] > RED_GOAL_CORNERS[1][1]))) out = (curr[0] > (RED_GOAL_CORNERS[2][0] + robot_radius) && TARGET[0] > (RED_GOAL_CORNERS[2][0] + robot_radius)) ? out : 1;
    
    return out;
}

double (*pathfind(double *current_pos, double * TARGET))[2] {
    static double path[16][2] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1},{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
    bool target_reached = 0;
    // The critical points around the map. There are 8 (A-H). These are known points at which
    // the bot can fit around features. This is a much quicker pathfinding method than raycasting.
    double critical_points[8][2] = {{225.0, 25.0}, {225.0, 75.0}, {225.0, 225.0}, {225.0, 275.0}, {75.0, 275.0}, {75.0, 225.0}, {75.0, 75.0}, {75.0, 25.0}};
    int num_zones = sizeof(critical_points) / 16;
    int zone_diff = num_zones / 2;
    int curr_zone = in_zone(current_pos);
    int targ_zone = in_zone(TARGET);
    cout << "TARGET is in Zone " << targ_zone << endl;
    int i = 1;
    double move_point[] = {current_pos[0], current_pos[1]};
    int direction;
    if (((targ_zone - curr_zone) <= zone_diff && (targ_zone - curr_zone) > 0) || ((targ_zone - curr_zone) <= -(zone_diff) && (targ_zone - curr_zone) > -(2 * zone_diff))) direction = 1;
    else direction = -1;
    
    while (!target_reached) {
        bool obstructed_path = 0;        
        obstructed_path = obstructed(move_point, TARGET);
        if (obstructed_path) cout << "Path obstructed" << endl;
        if ((curr_zone == targ_zone) || !obstructed_path) {
            cout << "Found TARGET" << endl;
            path[i][0] = TARGET[0];
            path[i][1] = TARGET[1];
            curr_zone = in_zone(TARGET);
            target_reached = 1;
            break;
        }
        int next_zone = curr_zone + direction;
        if (next_zone >= num_zones) next_zone = 0;
        else if (next_zone < 0) next_zone = 7;
        double mid_target[] = {critical_points[next_zone][0], critical_points[next_zone][1]};
        bool mid_obstruct = obstructed(move_point, mid_target);
        if (mid_obstruct) {
            cout << "Mid obstructed at increment " << i - 1 << endl;
            path[i][0] = critical_points[curr_zone][0];
            path[i][1] = critical_points[curr_zone][1];
            if (path[i - 1][0] == path[i][0] && path[i - 1][1] == path[i][1]) --i;
            ++i;
        }
        path[i][0] = critical_points[next_zone][0];
        path[i][1] = critical_points[next_zone][1];

        move_point[0] = path[i][0];
        move_point[1] = path[i][1];
        curr_zone = in_zone(move_point);
        cout << "Zone at end of path is " << curr_zone << endl;
        ++i;
    }
    return path;
}
    
int main() {

    double local_rec[2], local_angle;
    cout << "Where is the robot? (x, y)" << endl;
    cin >> local_rec[0];
    cin >> local_rec[1];
    //cout << "What direction is the robot facing? (Angle from 0 in RAD)" << endl;
    //cin >> local_angle;

    double *local_pol = rec2pol(local_rec);

    cout << "Robot is at (" << local_rec[0] << ", " << local_rec[1] << ")" << endl;

    double current_pos[] = {local_rec[0], local_rec[1], -1};

    int zone = in_zone(current_pos);

    current_pos[2] = zone;

    cout << "Current position in Zone " << zone << endl;

    cout << "Where is the TARGET (x, y)" << endl;
    double TARGET[2];
    cin >> TARGET[0];
    cin >> TARGET[1];

    cout << "TARGET is at (" << TARGET[0] << ", " << TARGET[1] << ")" << endl;


    double (*path)[2]; // Create a dynamic vector to store coordinates from *path that are not (-1, -1)
    path = pathfind(current_pos, TARGET);

    cout << "The path steps are: " << endl;
    cout << "START: (" << current_pos[0] << ", " << current_pos[1] << ")" << endl;
    for (int i = 1; i < 16; ++i) {
        cout << "(" << path[i][0] << ", " << path[i][1] << ")" << endl;
        if (path[i + 1][0] == -1) {
            cout << "TARGET Reached" << endl;
            break;
        }
    }

    system("pause");
    return 0;
}