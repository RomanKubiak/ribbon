#pragma once
#include "AnalogInput.h"
#include "Buttons.h"
#include "Menu.h"
#include "Quantize.h"

namespace ribbon {
    class Manager {
    public:
        Manager(Adafruit_SH1107 &display, Encoder &encoder, WS2812Serial &leds);

        void begin();
        void update();
        ribbon::Quantize quantize;
        ribbon::AnalogInput &getAnalogInput() { return analogInput; }
    private:
        ribbon::Menu ribbonMenu;
        ribbon::AnalogInput analogInput;
        ribbon::Buttons buttons;

    };
};