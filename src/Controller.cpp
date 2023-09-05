#include "Controller.h"

Controller::Controller(vex::controllerType controller_type){
    controller(controller_type);

}

Controller::Button::Button(Controller::button_type id, Controller *parent ) {
    controller::button(id, parent);
} 

Controller::Axis::Axis(const vex::axisType id, Controller *parent) {
    controller::axis(id, parent);
}

void Controller::record_macro(bool IS_MACRO_RECORDING) {}

void Controller::run_macro(bool IS_MACRO_RUNNING) {}

bool Controller::is_recording() {}

bool Controller::is_running() {}

bool Controller::Button::pressing() {}

int Controller::Axis::value() {}

