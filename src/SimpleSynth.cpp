#include "../include/SimpleSynth.h"

extern ribbon::Manager manager;
namespace ribbon {
    void SimpleSynth::begin() {
        outputMixer.gain(2, 0.01);
        outputMixer.gain(3, 0.0);
        wave.begin(1.0, 80, WAVEFORM_SAWTOOTH);
        filterInputMixer.gain(0, 0.7);
        filterInputMixer.gain(1, 0.4);
        filter.frequency(440);
        filter.resonance(12.0);
        env.attack(0.0);
        env.decay(100.0);
        env.sustain(0.5);
        env.release(10.0);
        manager.quantize.addListener(ribbon::QuantizedValues::noteNumber, this);
        manager.quantize.addListener(ribbon::QuantizedValues::velocity, this);
        manager.quantize.addListener(ribbon::QuantizedValues::noteOnOff, this);
    }

    void SimpleSynth::quantizedValueChanged(const QuantizedValues type, const int16_t value) {
        if (type == ribbon::QuantizedValues::noteNumber) {
            wave.frequency(ribbon::noteMidiInfo::noteFrequencies[value]);
        }

        if (type == ribbon::QuantizedValues::velocity) {
            filterInputMixer.gain(1, map<float>(value, 0, 127, 0.1f, 0.6f));
        }

        if (type == ribbon::QuantizedValues::noteOnOff) {
            if (value == 0) {
                /* note off */
                env.noteOn();
                dc.amplitude(1.0, 100);
            } else {
                /* note on */
                env.noteOff();
                dc.amplitude(0.0);
            }
        }
    }
};