#pragma once
#include "vex.h"

/// @brief    Wrapper for the vex::controller class to allow for storing and reading the
///           controller button values (macros).
class Controller : public vex::controller
{
public:
    Controller(vex::controllerType controller_type);

};