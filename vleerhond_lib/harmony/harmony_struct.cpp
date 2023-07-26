#include "harmony/harmony_struct.h"

#include "core/defs.h"
#include "harmony/chords.h"
#include "harmony/scale.h"
#include "patterns/cv_patterns.h"
#include "utils/rand.h"
#include "utils/utils.h"

namespace Vleerhond
{
HarmonyStruct::HarmonyStruct()
{
    type              = HarmonyType::Const;
    this->const_value = 0;
}

ChordStruct HarmonyStruct::getChordStep(const TimeStruct& time) const
{
    switch(this->type)
    {
        case HarmonyType::Const:
            return ChordStruct(this->const_value, ChordType::TRIAD);
        case HarmonyType::TonicLow:
            return this->low_tonic_pattern.getChord(time);
        case HarmonyType::TonicHigh:
            return this->high_tonic_pattern.getChord(time);
        case HarmonyType::DominantLow:
            return this->low_dominant_pattern.getChord(time);
        case HarmonyType::DominantHigh:
            return this->high_dominant_pattern.getChord(time);
        default:
            return ChordStruct(this->const_value, ChordType::TRIAD);
    }
}

void HarmonyStruct::randomize()
{
    // Set high patterns
    this->randomizeHighPattern(this->low_tonic_pattern, 0, false);
    this->randomizeHighPattern(this->high_tonic_pattern, 0, true);
    this->randomizeHighPattern(this->low_dominant_pattern, 4, false);
    this->randomizeHighPattern(this->high_dominant_pattern, 4, true);
}

void HarmonyStruct::randomizeHighPattern(ChordPattern& high_pattern,
                                         const uint8_t start_chord,
                                         const bool    long_pattern)
{
    high_pattern.setChordPattern(this->scale, start_chord, long_pattern);

    switch(Rand::distribution(10, 10))
    {
        case 0: high_pattern.time_division = TimeDivision::Quarter; break;
        case 1: high_pattern.time_division = TimeDivision::Half; break;
    }
}

void HarmonyStruct::switchConstChord()
{
    std::vector<uint8_t> options = this->scale.getAvailableChordsIndices();
    std::vector<uint8_t> new_options;
    for (auto& o : options)
    {
        if (o != const_value && o != 0)
        {
            new_options.push_back(o);
        }
    }
    this->const_value = options[Rand::randui8(options.size())];
}
uint16_t HarmonyStruct::getPatternLength(HarmonyType harmony_type)
{
    switch(harmony_type)
    {
        case HarmonyType::Const: return 16 * (uint16_t)TimeDivision::Sixteenth;
        case HarmonyType::TonicLow:
            return this->low_tonic_pattern.length
                   * (uint16_t)low_tonic_pattern.time_division;
        case HarmonyType::TonicHigh:
            return this->high_tonic_pattern.length
                   * (uint16_t)high_tonic_pattern.time_division;
        case HarmonyType::DominantLow:
            return this->low_dominant_pattern.length
                   * (uint16_t)low_dominant_pattern.time_division;
        case HarmonyType::DominantHigh:
            return this->high_dominant_pattern.length
                   * (uint16_t)high_dominant_pattern.time_division;
    }
    return 1;
}

void HarmonyStruct::setType(HarmonyType type)
{
    this->type = type;
}

HarmonyType HarmonyStruct::getType()
{
    return this->type;
}

void HarmonyStruct::setTonic()
{
    setType(HarmonyType::Const);
    const_value = 0;
}

} // namespace Vleerhond