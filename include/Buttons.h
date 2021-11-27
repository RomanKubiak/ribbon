#pragma once

#include <vector>
#include "config.h"
#include "../lib/ClickButton/ClickButton.h"
#include "../lib/Bounce2/src/Bounce2.h"

namespace ribbon {
    enum class LowLevelHandlerClass {
        click,
        bounce
    };

    class ButtonInput {
    public:
        ButtonInput(const ribbon::LowLevelHandlerClass _lowLevelHandler,
                    int pin,
                    std::string _name,
                    const bool activeType = LOW,
                    const bool interalPullup = true)
                : lowLevelHandler(_lowLevelHandler), name(_name) {
            if (lowLevelHandler == LowLevelHandlerClass::click)
                clickButton = new ClickButton(pin, activeType, interalPullup);
            else {
                bounce2 = new Bounce();
                bounce2->attach(pin, INPUT_PULLUP);
            }
        }

        ~ButtonInput() {}
        bool clickDetected()
        {
            if (lowLevelHandler == LowLevelHandlerClass::click) {
                return clickButton->changed;
            }

            if (lowLevelHandler == LowLevelHandlerClass::bounce) {
                return bounce2->fell();
            }

            return false;
        }

        bool isDown()
        {
            if (lowLevelHandler == LowLevelHandlerClass::click) {
                return clickButton->changed;
            }

            if (lowLevelHandler == LowLevelHandlerClass::bounce) {
                return bounce2->fell();
            }

            return false;
        }

        bool isUp()
        {
            if (lowLevelHandler == LowLevelHandlerClass::click) {
                return clickButton->changed;
            }

            if (lowLevelHandler == LowLevelHandlerClass::bounce) {
                return bounce2->rose();
            }

            return false;
        }

        bool wasHeld() {
            if (lowLevelHandler == LowLevelHandlerClass::click) {
                return clickButton->clicks == -1;
            }

            if (lowLevelHandler == LowLevelHandlerClass::bounce) {
                return false;
            }

            return false;
        }
        LowLevelHandlerClass lowLevelHandler;
        std::string name;
        ClickButton *clickButton;
        Bounce *bounce2;
        std::vector <std::function<void(ButtonInput &)>> callbacks;
    };

    enum ButtonsOnBoard {
        Right,
        Left,
        Up,
        Down,
        SequencerButton,
        EncoderButton,
        NumButtons
    };

    class Manager;

    class Buttons {
    public:
        void begin();
        void update();

        void buttonChanged(ribbon::ButtonInput &button);

        void setCallback(const ButtonsOnBoard btn, std::function<void(ButtonInput &)> cbk);

        ribbon::ButtonInput &getByName(const std::string &buttonName) {
            for (uint8_t i=0; i<buttons.size(); i++) {
                if (buttons[i].name == buttonName) {
                    return buttons[i];
                }
            }
            return buttons[0];
        }

        std::vector<ribbon::ButtonInput> buttons = {
                {ribbon::LowLevelHandlerClass::bounce, BLACK_BUTTON1,     "Black button left"},
                {ribbon::LowLevelHandlerClass::bounce, BLACK_BUTTON2,     "Black button right"},
                {ribbon::LowLevelHandlerClass::bounce, SWITCH_BOTTOM,     "Lower switch"},
                {ribbon::LowLevelHandlerClass::bounce, SWITCH_TOP,     "Upper switch"}
        };
    };

};