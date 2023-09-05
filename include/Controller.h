#pragma once
#include "vex.h"

/// @brief    Wrapper for the vex::controller class to allow for storing and reading the
///           controller button values (macros).
class Controller : public vex::controller
{
public:
    Controller(vex::controllerType controller_type);

    // TODO: may need to declare each button

    /// @brief    Starts or stops the macro-recording. Won't start if running.
    /// @param IS_MACRO_RECORDING    True to start/continue the recording, false to stop. 
    void record_macro(bool IS_MACRO_RECORDING);

    /// @brief Starts or stops the macro-running. Won't start if recording.
    /// @param IS_MACRO_RUNNING     True to start/continue running, false to stop. 
    void run_macro(bool IS_MACRO_RUNNING);

    /// @brief    State of recording.
    /// @return   True if currently recording, else false.
    bool is_recording();

    /// @brief   State of macro running.
    /// @return  True if currently running, else false.
    bool is_running();

    enum class button_type {
        kButtonL1 = 0,
        kButtonL2,
        kButtonR1,
        kButtonR2,
        kButtonUp,
        kButtonDown,
        kButtonLeft,
        kButtonRight,
        kButtonX,
        kButtonB,
        kButtonY,
        kButtonA,

        kButtonRes1,
        kButtonRes2,

        kButtonUndefined
    };

    class Button : public vex::controller::button {
    public:
        Button(const Controller::button_type id, Controller *parent);

        /// @brief    If IS_MACRO_RECORDING, returns and stores the button state. Else if
        ///           IS_MACRO_RUNNING, returns the first value in input not already run. Else
        ///           just returns the button state.
        /// @return    Returns true if button (running or recorded) is pressed, else false.
        bool pressing(); 

    private:
        std::vector<int> input{};
    };

    class Axis : public vex::controller::axis {
    public:
        Axis(const vex::axisType id, Controller *parent);
        /// @brief    If IS_MACRO_RECORDING, returns and stores the axis state. Else if
        ///           IS_MACRO_RUNNING, returns the first value in input not already run. Else
        ///           just returns the button state.
        /// @return    Returns axis value (running or recorded) from -127 to 127.
        int value(); 
    private:
        std::vector<int> input{};
    };

    const Button ButtonA = Button(button_type::kButtonA, this );

private:
    bool IS_MACRO_RECORDING = false;
    bool IS_MACRO_RUNNING = false;
    


};