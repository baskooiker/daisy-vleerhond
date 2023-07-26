#pragma once

#include "core/defs.h"
#include "core/timing_structs.h"

namespace Vleerhond {
class TimeStruct {
   public:
    uint32_t tick;
    PlayState state;
    uint32_t last_pulse_time;
    float average_pulse_time;
    uint8_t global_shuffle;
    uint64_t ticks_per_step;

   public:
    TimeStruct();
    uint32_t step() const;
    uint32_t getShuffleDelay(
        const int8_t shuffle_offset = 0, const uint8_t global_delay = 0) const;
    uint32_t getShuffleDelay(const MicroTimingStruct& micro) const;
    uint32_t getCount(const TimeDivision time_division) const;
    void randomizeShuffle();
    uint8_t ticksLeftInBar() const;
    void reset();
    TimeStruct add(const int32_t offset);
};
}  // namespace Vleerhond