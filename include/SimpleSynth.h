#pragma once

#include <Audio.h>
#include "Quantize.h"
#include "Manager.h"

namespace ribbon {
    class SimpleSynth : public Quantize::Listener {
    public:
        SimpleSynth(AudioSynthWaveform &_wave,
                    AudioFilterStateVariable &_filter,
                    AudioEffectEnvelope &_env,
                    AudioMixer4 &_outputMixer,
                    AudioAmplifier &_fbkAmp,
                    AudioMixer4 &_filterInputMixer,
                    AudioSynthWaveformDc &_dc)
                : wave(_wave),
                  filter(_filter),
                  env(_env),
                  outputMixer(_outputMixer),
                  fbkAmp(_fbkAmp),
                  filterInputMixer(_filterInputMixer),
                  dc(_dc)
                  {}

        void begin();

        void quantizedValueChanged(const QuantizedValues type, const int16_t value);

    private:
        AudioSynthWaveform &wave;
        AudioFilterStateVariable &filter;
        AudioEffectEnvelope &env;
        AudioMixer4 &outputMixer;
        AudioAmplifier &fbkAmp;
        AudioMixer4 &filterInputMixer;
        AudioSynthWaveformDc &dc;
    };
};