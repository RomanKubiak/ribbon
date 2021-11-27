//
// Created by kbar on 13.11.2021.
//
#include <Audio.h>
#include "Granular.h"
extern AudioEffectGranular granular1;

void Granular::begin() {
    granular1.begin(granularMemory, GRANULAR_MEM_SIZE);
}

void Granular::updateGranular(uint16_t value) {
    float ratio;
    ratio = powf(2.0, (float) (value / 1023.0) * 2.0 - 1.0);
    granular1.setSpeed(ratio);
}

void Granular::updatePitchShift(uint16_t value) {
    pitchShiftMsec = 25.0 + ((float) (value / 1023.0) * 75.0);
    granular1.beginPitchShift(pitchShiftMsec);
}