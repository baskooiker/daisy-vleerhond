#include "instruments/tonal_instruments/poly.h"

#include "midi/midi_io.h"
#include "utils/rand.h"

namespace Vleerhond {
Poly::Poly(HarmonyStruct& harmony, TimeStruct& time)
    : TonalInstrumentBase(harmony, time) {
    pitch_offset = 36;
    type = PolyType::PolyLow;
    // randomize();
}

void Poly::randomize() {
    // ofLogVerbose("poly", "randomize()");
    last_randomized_time = Utils::millis();

    // Set pattern low
    this->gates_low.setGatesLow();

    // Set pattern high
    uint8_t steps = Rand::randui8(5, 11);
    this->gates.setEuclid(16, steps);
    this->gates.abPattern.randomize();

    // Set Tie Pattern
    this->tie_pattern.randomize(Rand::randf(.5f, .99f));

    // Randomize pitch range
    // TODO: make min- and max- parameterizable.
    this->pitch_offset = Rand::randui8(40, 64);

    switch (Rand::distribution(16, 16)) {
        case 0:
            this->type = PolyType::PolyHigh;
            break;
        case 1:
            this->type = PolyType::PolyLow;
            break;
    }
}

bool Poly::play() {
    bool hit = false;
    switch (this->type) {
        case PolyType::PolyLow:
            hit = this->gates_low.gate(time);
            break;
        case PolyType::PolyHigh:
            hit = this->gates.gate(time);
            break;
    }

    if (hit) {
        ChordStruct chord_struct = harmony.getChordStep(time);

        uint8_t total_pitch_offset = Utils::rerange(
            this->_variable_pitch_offset, 36, this->pitch_offset);
        std::vector<uint8_t> chord_notes = ChordUtils::getChord(
            chord_struct, harmony.scale, total_pitch_offset);

        uint8_t length = 6;
        if (this->tie_pattern.gate(time) || this->type == PolyType::PolyLow) {
            length = time.ticksLeftInBar();
        }

        // TODO: This is a testing hack...
        // length = 24;

        std::vector<NoteStruct> note_structs;
        for (size_t i = 0; i < chord_notes.size(); i++) {
            NoteStruct note(chord_notes[i], 64, length, NoteType::Tie);
            note_structs.push_back(note);
        }

        this->getChannel()->noteOn(note_structs, 0);
        return true;
    }
    return false;
}

bool Poly::getPedal() { return false; /*tie_pattern.gate(time);*/ }
}  // namespace Vleerhond
