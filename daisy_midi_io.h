#pragma once

#include "midi/midi_io.h"
#include "daisy_patch.h"

namespace Vleerhond::Midi {

void Init(daisy::MidiUartHandler* midi);

}