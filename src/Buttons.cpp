#include "Buttons.h"

namespace ribbon {

    void Buttons::begin() {
        pinMode(BLACK_BUTTON1, INPUT_PULLUP);
        pinMode(BLACK_BUTTON2, INPUT_PULLUP);
        pinMode(SWITCH_BOTTOM, INPUT_PULLUP);
        pinMode(SWITCH_TOP, INPUT_PULLUP);
    }

    void Buttons::update() {
        for (auto &button: buttons) {
            if (button.lowLevelHandler == ribbon::LowLevelHandlerClass::click) {
                button.clickButton->Update();

                if (button.clickButton->clicks != 0)
                    buttonChanged(button);
            }

            if (button.lowLevelHandler == ribbon::LowLevelHandlerClass::bounce) {
                button.bounce2->update();

                if (button.bounce2->fell() || button.bounce2->rose())
                    buttonChanged(button);
            }
        }
    }

    void Buttons::buttonChanged(ButtonInput &button) {
        printf("*** btn: %s -> %s\r\n", button.name.c_str(), button.isDown() ? "DOWN" : "UP");
        for (auto &call: button.callbacks)
            call(button);
    }

    void Buttons::setCallback(const ButtonsOnBoard btn, std::function<void(ButtonInput &)> cbk) {
        buttons[btn].callbacks.push_back(cbk);
    }
}