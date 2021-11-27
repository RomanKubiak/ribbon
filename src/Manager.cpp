//
// Created by kbar on 19.11.2021.
//

#include "Manager.h"

namespace ribbon {
    Manager::Manager(Adafruit_SH1107 &display, Encoder &encoder, WS2812Serial &leds)
    : quantize(leds), ribbonMenu(display, encoder)
    {
    }

    void Manager::begin() {
        ribbonMenu.begin();
        analogInput.begin();
        buttons.begin();
        quantize.begin();

        analogInput.addListenerForPot(PotsOnBoard::RibbonPosition, &quantize);
        analogInput.addListenerForPot(PotsOnBoard::RibbonPressure, &quantize);
    }

    void Manager::update() {
        analogInput.update();
    }
};