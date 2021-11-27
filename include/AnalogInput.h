#pragma once
#include "config.h"
#include <string>
#include <ADC.h>
#include <vector>
#include <map>

class RibbonAnalProvier {
public:
    RibbonAnalProvier(const uint8_t _pin) : pin(_pin), changed(false), average(0) {}
    void begin() { pinMode(pin, INPUT); }
    void update() {
        average = 0;
        for (int i = 0; i<sampleCount ;i++) {
            average += analogRead(pin);
        }
        const uint16_t currentValue = average / sampleCount;

        if (currentValue != lastValue) {
            lastValue = currentValue;
            changed = true;
        } else {
            changed = false;
        }
    }
    const uint16_t getValue() const
    {
        return lastValue;
    }
    const bool hasChanged() {
        const bool ret = changed;
        changed = false;
        return ret;
    }
    uint8_t pin;
    bool changed;
    unsigned long average;
    uint16_t lastValue;
    static constexpr auto sampleCount = 256;
};

namespace ribbon {
    class AnalogInputListener;

    enum PotsOnBoard {
        RibbonPosition,
        RibbonPressure
    };

    struct PotInput {
        PotsOnBoard type;
        RibbonAnalProvier provider;
    };

    class AnalogInputListener {
    public:
        virtual void analogInputChanged(const PotInput &pot) = 0;
    };

    class AnalogInput {
    public:
        AnalogInput();
        ~AnalogInput() {}
        void begin();
        void update();
        void addListenerForPot(const ribbon::PotsOnBoard potOnBoard, AnalogInputListener *listenerToAdd = nullptr);

    private:
        std::multimap <const ribbon::PotsOnBoard, AnalogInputListener *> listeners;
        std::vector<PotInput> pots;
        elapsedMicros microUpdate;
    };
};