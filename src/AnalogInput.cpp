#include "AnalogInput.h"
#include <algorithm>

typedef std::multimap<const ribbon::PotsOnBoard, ribbon::AnalogInputListener *>::iterator AnalPenetrator;

namespace ribbon {
    AnalogInput::AnalogInput() : pots { { RibbonPosition, RibbonAnalProvier(A0) },
                                        { RibbonPressure, RibbonAnalProvier(A1) }} {
        for (auto &pot: pots) {
            pot.provider.begin();
        }
    }

    void AnalogInput::begin() {}

    void AnalogInput::update() {
        for (auto &pot: pots) {
            pot.provider.update();
            if (pot.provider.hasChanged()) {
                std::pair<AnalPenetrator, AnalPenetrator> result = listeners.equal_range(pot.type);
                for (AnalPenetrator it = result.first; it != result.second; it++) {
                    it->second->analogInputChanged(pot);
                }
            }
        }

    }

    void AnalogInput::addListenerForPot(const ribbon::PotsOnBoard potOnBoard, AnalogInputListener *listenerToAdd) {
        listeners.emplace(std::make_pair(potOnBoard, listenerToAdd));
    }

}