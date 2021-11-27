#pragma once

#include <vector>
#include <string>
#include <WS2812Serial.h>
#include "AnalogInput.h"


namespace ribbon {
    enum QuantizedValues {
        noteNumber,
        velocity,
        noteOnOff
    };
    namespace noteMidiInfo {
        const std::vector<double> noteFrequencies = { 16.35, 17.32, 18.35, 19.45, 20.60, 21.83,
                                                      23.12, 24.50, 25.96, 27.50, 29.14, 30.87,
                                                      32.70, 34.65, 36.71, 38.89, 41.20, 43.65,
                                                      46.25, 49.00, 51.91, 55.00, 58.27, 61.74 };
    }
    namespace offsets {
        static constexpr auto bottomFirst = 64;
        static constexpr auto bottomLast = 34;

        static constexpr auto topFirst = 3;
        static constexpr auto topLast = 35;

        static constexpr auto octaveOptions = 4;
    }
    namespace colors {
        static constexpr auto whiteKey = 0x041204;
        static constexpr auto blackKey = 0x100000;

        static constexpr auto whiteKeyDown = 0x145914;
        static constexpr auto blackKeyDown = 0x400000;

        static constexpr uint32_t octaveColors[offsets::octaveOptions] = { 0x000606, 0x000606, 0x000606, 0x000606 };

        static const uint32_t fade(const uint32_t color, const float amount=0.4f) {
            const uint8_t b = (color & 255) * amount;
            const uint8_t g = ((color >> 8) & 255) * amount;
            const uint8_t r = ((color >> 16) & 255) * amount;

            return (r << 16) | (g << 8) | b;
        }
    };
    namespace pianoKeys {
        static constexpr auto white = 0;
        static constexpr auto black = 1;
        static const std::vector<int> octave = {white, black, white, black, white, white,
                                                black, white, black, white, black, white,
                                                white, black, white, black, white, white,
                                                black, white, black, white, black, white};
    }

    class Quantize : public ribbon::AnalogInputListener {
    public:
        class Listener {
        public:
            virtual void quantizedValueChanged(const QuantizedValues type, const int16_t value) = 0;
        };
        Quantize(WS2812Serial &_leds);
        void drawPianoKeysUsingLeds(uint8_t start = 2);
        static auto const minAnalogNote = 100;
        static auto const maxAnalogNote = 901;
        static auto const minAnalogOctave = 918;
        static auto const maxAnalogOctave = 1010;
        static auto const pianoKeysOffsetInLeds = 2;
        const std::vector<std::string> noteNames = {"C", "C#", "D", "D#", "E", "F",
                                                    "F#", "G", "G#", "A", "A#", "B",
                                                    "C", "C#", "D", "D#", "E", "F",
                                                    "F#", "G", "G#", "A", "A#", "B"};


        const std::vector<std::pair<uint16_t, uint16_t>> analogRangesForOctaves = {
                {918,943},
                {944,967},
                {968,993},
                {994,1009}
        };

        const std::vector<std::pair<uint16_t, uint16_t>> analogRangesForNotes = {
                {100,190},
                {191,240},
                {241,304},
                {305,348},
                {349,391},
                {392,439},
                {440,471},
                {472,509},
                {510,538},
                {539,571},
                {572,592},
                {593,623},
                {624,654},
                {655,673},
                {674,700},
                {701,717},
                {718,740},
                {741,763},
                {764,784},
                {785,807},
                {808,824},
                {825,848},
                {849,866},
                {867,901}
        };
        const uint16_t findNote(const uint16_t analogPosition);
        const int8_t findOctave(const uint16_t analogPosition);
        void begin();
        void setPosition(const uint16_t analogPosition);
        void setPressure(const uint16_t analogPressure);
        void analogInputChanged (const PotInput &pot);
        const bool isFingerDown();
        void notifyQuantizationListeners(const QuantizedValues valueThatChanged);
        void addListener(const ribbon::QuantizedValues value, ribbon::Quantize::Listener *listenerToAdd);
    private:
        WS2812Serial &leds;
        int16_t currentKeyDown = -1;
        int8_t currentOctaveDown = -1;
        volatile bool fingerIsDown = false;
        uint16_t velocity;
        std::multimap <const ribbon::QuantizedValues, ribbon::Quantize::Listener *> listeners;
    };
};