#pragma once

#include <string>

#include "core/enums.h"

namespace Vleerhond {
class Strings {
   public:
    static std::string getString(const ScaleType scale) {
        // switch (scale) {
        //     case ScaleType::AEOLIAN:
        //         return "aeolian";
        //     case ScaleType::DORIAN:
        //         return "dorian";
        //     case ScaleType::IONIAN:
        //         return "ionian";
        //     default:
        //         return "unkown scale";
        // }
        return "";
    }

    static std::string getString(const Root root) {
        switch (root) {
            case Root::ROOT_C:
                return "C";
            case Root::ROOT_C_SHARP:
                return "C#";
            case Root::ROOT_D:
                return "D";
            case Root::ROOT_D_SHARP:
                return "D#";
            case Root::ROOT_E:
                return "E";
            case Root::ROOT_F:
                return "F";
            case Root::ROOT_F_SHARP:
                return "F#";
            case Root::ROOT_G:
                return "G";
            case Root::ROOT_G_SHARP:
                return "G#";
            case Root::ROOT_A:
                return "A";
            case Root::ROOT_A_SHARP:
                return "A#";
            case Root::ROOT_B:
                return "B";
            default:
                return "unkown root";
        }
        // return "";
    }

    static std::string getString(const ChordStruct& chord) {
        auto r = getString(static_cast<Root>(chord.root % 12));
        switch(chord.chord_type)
        {
            case ChordType::TRIAD:
                return r;
            case ChordType::SUS2:
                return r + "SUS2";
            case ChordType::SUS4:
                return r + "SUS4";
            case ChordType::ADD6:
                return r + "6";
            case ChordType::SEVEN:
                return r + "7";
        } 
        return r + "Unkown";
        // return "";
    }

    static const std::string getString(const MonoStyle style) {
        switch (style) {
            case MonoStyle::MonoPolyRhythm:
                return "poly_rhythm";
            case MonoStyle::MonoSixteenths:
                return "sixteenths";
            default:
                return "invalid style";
        }
    }
};
}  // namespace Vleerhond
