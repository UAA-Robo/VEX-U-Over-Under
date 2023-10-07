// GUI CONTROLS
//
// TAB - switch between show pathfinding history mode (default) and simple show path mode
// SPACE - randomly generate two points and generate the path or path history between them, depending on the mode
// RIGHT CLICK - clear current path and allows user to click to nodes to show the path between
// LEFT CLICK - selects node to use in pathfinding (max 2 nodes)
// UP ARROW - (during pathfinding histoy mode) automatically steps through the pathfinding history
// DOWN ARROW - (during pathfinding histoy mode) stops the auto mode
// LEFT ARROW - manually go to the previous snapshot
// RIGHT ARROW - manually got to the next snapshot

#include <vector>
#include <set>
#include "Graph.h"
#include <SDL2/SDL.h>

class GUI
{
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  Graph *const GRAPH;
  const int VISUAL_NODE_SIZE;
  const int X_NODES_COUNT_COUNT;
  const int Y_NODES_COUNT_COUNT;
  const int GRID_WIDTH;
  const int GRID_HEIGHT;
  const int X_PADDING;
  const int Y_PADDING;
  const int SCROLL_SPEED;
  int y_offset;
  int selected_nodes_count;
  int snapshot_number;
  int paths_found_count;
  int total_computation_duration;
  GUIMode mode;
  bool is_selecting_nodes_allowed;
  bool is_auto_mode;
  bool has_data_to_draw;
  SDL_bool quit;
  SDL_bool mouse_active;
  SDL_bool mouse_hover;

  Node *a;
  Node *b;
  Node *selected_node_waypoints_neighbors;

  std::vector<Node *> *forbidden_nodes;
  std::vector<Node *> *selected_nodes;
  std::vector<Node *> *path_nodes;
  std::vector<std::vector<Node *> *> *path_nodes_snapshots;
  std::set<Node *> *waypoints;

  SDL_Rect cursor;
  const SDL_Color BACKGROUND_COLOR;
  const SDL_Color LINE_COLOR;
  const SDL_Color CURSOR_COLOR;
  const SDL_Color FORBIDDEN_CORE_NODE_COLOR;
  const SDL_Color FORBIDDEN_ROBOT_NODE_COLOR;
  const SDL_Color FORBIDDEN_BUFFER_NODE_COLOR;
  const SDL_Color PATH_NODE_COLOR;
  const SDL_Color PAST_PATH_NODE_COLOR;
  const SDL_Color ORIGIN_NODE_COLOR;
  const SDL_Color DESTINATION_NODE_COLOR;
  const SDL_Color WAYPOINT_COLOR;

  int run();
  void event_loop();
  void clear_data();
  void get_path(Node *a, Node *b);
  void get_random_path();
  void get_path_snapshots(Node *a, Node *b);
  void get_random_path_snapshots();
  void generate_path();
  void change_modes();
  void reset_grid();
  void select_node(SDL_Event &event);
  void move_cursor(SDL_Event &event);
  void scroll(SDL_Event &event);
  void draw_background();
  void draw_grid();
  void draw_forbidden_nodes();
  void draw_main();
  void draw_path();
  void draw_normal_path();
  void draw_vg_path();
  void draw_snapshots();
  void draw_normal_snapshots();
  void draw_vg_snapshots();
  void draw_LOS();
  // void draw_LOS(Node *a, Node *b);
  void draw_cursor();
  void draw_vector_nodes(std::vector<Node *> nodes, SDL_Color pathColor, SDL_Color startingNodeColor, SDL_Color endingNodeColor);
  void draw_line(SDL_Renderer *renderer, Node *a, Node *b, SDL_Color color);
  void draw_line(SDL_Renderer *renderer, Node *a, Node *b);
  void draw_lines_between_nodes(std::vector<Node *> nodes, SDL_Color pathColor);
  bool is_inside_node(Node *node, int eventX, int eventY);
  void draw_waypoints();
  void draw_cell(SDL_Renderer *renderer, Node *node, SDL_Color color);
  void draw_cell(SDL_Renderer *renderer, Node *node);
  void draw_cellSecond(SDL_Renderer *renderer, Node *node, SDL_Color color, int xDifference);
  void get_random_nodes();
  void draw_neighboring_waypoints(SDL_Event &event);

public:
  GUI(Graph *graph, int visual_node_size);
};
