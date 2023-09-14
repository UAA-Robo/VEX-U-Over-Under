#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "Node.h"
#include "Graph.h"

class Button
{
private:
  std::vector<Node *> (*func)(Graph *graph);
  Graph *graph;

public:
  SDL_Rect rect;
  SDL_Color color;
  bool clicked;

  Button(std::vector<Node *> (*func)(Graph *graph), Graph *graph, SDL_Rect rect, SDL_Color color)
  {
    this->func = func;
    this->rect = rect;
    this->color = color;
  }

  std::vector<Node *> HandleEvent(const SDL_Event *event)
  {
    if (event->type == SDL_MOUSEBUTTONDOWN &&
        event->button.button == SDL_BUTTON_LEFT &&
        this->isInBounds(event->motion.x, event->motion.y))
    {
      this->clicked = true;
      return this->func(this->graph);
      // std::cout << event->motion.x << ", " << event->motion.y << "\t" << this->rect.x << ", " << this->rect.y << std::endl;
    }
    throw "A";
  }

  bool isInBounds(int x, int y)
  {
    if (((this->rect.x <= x) && (x <= this->rect.x + this->rect.w)) &&
        ((this->rect.y <= y) && (y <= this->rect.y + this->rect.h)))
    {
      return true;
    }
    return false;
  }
};