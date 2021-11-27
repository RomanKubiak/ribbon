#pragma once
#include <cstdint>
#include "config.h"

class Granular {
    public:
        void begin();
        void updateGranular(uint16_t value);
        void updatePitchShift(uint16_t value);
    private:
        int16_t granularMemory[GRANULAR_MEM_SIZE];
        float pitchShiftMsec = 0.0f;
};
