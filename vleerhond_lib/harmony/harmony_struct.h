#pragma once

#include "core/defs.h"
#include "harmony/chords.h"
#include "harmony/scale.h"
#include "patterns/chord_pattern.h"

namespace Vleerhond {
class HarmonyStruct {
   private:
    HarmonyType type = HarmonyType::Const;
    uint8_t const_value = 0;

   public:
    ChordPattern low_tonic_pattern;
    ChordPattern low_dominant_pattern;
    ChordPattern high_tonic_pattern;
    ChordPattern high_dominant_pattern;

    Scale scale;

    HarmonyStruct();
    ChordStruct getChordStep(const TimeStruct& time) const;
    void randomize();
    void switchConstChord();
    uint16_t getPatternLength(HarmonyType type);
    void setType(HarmonyType type);
    HarmonyType getType();
    void setTonic();

   private:
    void randomizeHighPattern(
        ChordPattern& high_pattern, const uint8_t start_chord,
        const bool long_pattern);
};
}  // namespace Vleerhond
