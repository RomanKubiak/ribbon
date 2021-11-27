//
// Created by kbar on 13.11.2021.
//

#include "Quantize.h"
#include <utility>

namespace ribbon {
    typedef std::multimap<const ribbon::QuantizedValues, ribbon::Quantize::Listener *>::iterator QuantizedPenetrator;

    Quantize::Quantize(WS2812Serial &_leds) : leds(_leds) {

    }

    void Quantize::drawPianoKeysUsingLeds(uint8_t start) {
        using namespace pianoKeys;
        using namespace offsets;
        using namespace colors;
        uint8_t key;
        for (key = 0; key < octave.size(); key++) {
            if (key == currentKeyDown) {
                leds.setPixel(bottomFirst - (key + start),
                              (octave[key] == white) ? whiteKeyDown : blackKeyDown);
                leds.setPixel(topFirst + (key + start),
                              (octave[key] == white) ? whiteKeyDown : blackKeyDown);
            } else {
                leds.setPixel(bottomFirst - (key + start),
                              (octave[key] == white) ? whiteKey : blackKey);
                leds.setPixel(topFirst + (key + start),
                              (octave[key] == white) ? whiteKey : blackKey);
            }
        }

        leds.setPixel(29, 0x0f);

        for (int i = 0; i < offsets::octaveOptions; i++) {
            leds.setPixel(30 + i, (i == currentOctaveDown) ? ribbon::colors::fade(colors::octaveColors[i], 3.5f)
                                                           : colors::octaveColors[i]);
        }
        leds.show();
    }

    void Quantize::begin() {
        drawPianoKeysUsingLeds(2);
    }

    const uint16_t Quantize::findNote(const uint16_t analogPosition) {
        for (size_t key = 0; key < analogRangesForNotes.size(); key++) {
            if (analogPosition >= analogRangesForNotes[key].first &&
                analogPosition <= analogRangesForNotes[key].second) {
                return key;
            }
        }
        return -1;
    }

    const int8_t Quantize::findOctave(const uint16_t analogPosition) {
        for (size_t key = 0; key < analogRangesForOctaves.size(); key++) {
            if (analogPosition >= analogRangesForOctaves[key].first &&
                analogPosition <= analogRangesForOctaves[key].second) {
                return key;
            }
        }
        return -1;
    }

    void Quantize::analogInputChanged(const PotInput &pot) {
        if (pot.type == ribbon::RibbonPressure) {
            if (pot.provider.getValue() > 530) {
                velocity = map(pot.provider.getValue(), 530, 1000, 0, 127);
                fingerIsDown = true;

                notifyQuantizationListeners(QuantizedValues::velocity);
                notifyQuantizationListeners(QuantizedValues::noteOnOff);
            } else {
                velocity = 0;
                fingerIsDown = false;
                notifyQuantizationListeners(QuantizedValues::noteOnOff);
            }
        }
        fingerIsDown = (analogRead(A1) > 512) ? true : false;
        if (pot.type == ribbon::RibbonPosition) {
            const uint16_t v = 1024 - pot.provider.getValue();
            if ((v > minAnalogOctave && v < maxAnalogOctave) && fingerIsDown) {

                const int16_t newOctave = findOctave(v);

                printf("[%d]: pos == %d octave == %d fingerIsDown == %s\r\n", pot.type, v, newOctave,
                       fingerIsDown ? "true" : "false");

                if (newOctave == -1)
                    return;

                if (currentOctaveDown != newOctave) {
                    currentOctaveDown = newOctave;

                    drawPianoKeysUsingLeds(pianoKeysOffsetInLeds);
                }
            }
            if ((v >= minAnalogNote && v <= maxAnalogNote) && fingerIsDown) {

                const int16_t newKeyDown = findNote(v);

                printf("[%d]: pos == %d note == %d velo == %d name == %s\r\n", pot.type, v, newKeyDown, velocity,
                       noteNames[newKeyDown].c_str());

                if (newKeyDown == -1)
                    return;

                if (currentKeyDown != newKeyDown) {
                    currentKeyDown = newKeyDown;
                    notifyQuantizationListeners(QuantizedValues::noteNumber);
                    drawPianoKeysUsingLeds(pianoKeysOffsetInLeds);
                }
            }
        }
    }

    void Quantize::addListener(const ribbon::QuantizedValues value, ribbon::Quantize::Listener *listenerToAdd) {
        listeners.emplace(std::make_pair(value, listenerToAdd));
    }

    void Quantize::notifyQuantizationListeners(const QuantizedValues valueThatChanged) {
        if (valueThatChanged == QuantizedValues::noteNumber) {
            std::pair<QuantizedPenetrator, QuantizedPenetrator> result = listeners.equal_range(valueThatChanged);
            for (QuantizedPenetrator it = result.first; it != result.second; it++) {
                it->second->quantizedValueChanged(valueThatChanged, currentKeyDown);
            }
        }

        if (valueThatChanged == QuantizedValues::velocity) {
            std::pair<QuantizedPenetrator, QuantizedPenetrator> result = listeners.equal_range(valueThatChanged);
            for (QuantizedPenetrator it = result.first; it != result.second; it++) {
                it->second->quantizedValueChanged(valueThatChanged, velocity);
            }
        }

        if (valueThatChanged == QuantizedValues::noteOnOff) {
            std::pair<QuantizedPenetrator, QuantizedPenetrator> result = listeners.equal_range(valueThatChanged);
            for (QuantizedPenetrator it = result.first; it != result.second; it++) {
                it->second->quantizedValueChanged(valueThatChanged, fingerIsDown ? 1 : 0);
            }
        }
    }
}