#pragma once

#include "patterns/ab.h"
#include "scale.h"
#include "utils/utils.h"

namespace Vleerhond {
namespace ChordUtils {
std::vector<uint8_t> getChordTimePatternShort();
std::vector<uint8_t> getChordTimePatternLong();
std::vector<uint8_t> getChordSeq(std::vector<uint8_t> options, uint8_t length);
std::vector<uint8_t> getChord(const ChordStruct& chord, const Scale& scale, uint8_t offset);
}  // namespace ChordUtils
}  // namespace Vleerhond