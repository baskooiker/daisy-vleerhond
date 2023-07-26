#include "daisy_midi_io.h"

#include "midi/midi_io.h"
#include <string>
#include "core/defs.h"

namespace Vleerhond
{

daisy::MidiUartHandler* _midi = nullptr;

void Midi::Init(daisy::MidiUartHandler* midi)
{
    _midi = midi;
}

void MidiIO::sendTimeClock()
{
    if (_midi == nullptr) return;
    uint8_t bytes[1] = {0xF8};
    _midi->SendMessage(bytes, 1);
}

void MidiIO::sendStart()
{
    if (_midi == nullptr) return;
    uint8_t bytes[1] = {0xFA};
    _midi->SendMessage(bytes, 1);
}

void MidiIO::sendContinue()
{
    if (_midi == nullptr) return;
    uint8_t bytes[1] = {0xFB};
    _midi->SendMessage(bytes, 1);
}

void MidiIO::sendStop()
{
    if (_midi == nullptr) return;
    uint8_t bytes[1] = {0xFC};
    _midi->SendMessage(bytes, 1);
}

void MidiIO::sendNoteOn(const uint8_t pitch,
                        const uint8_t velocity,
                        const uint8_t channel)
{
    if (_midi == nullptr) return;
    uint8_t bytes[3] = {0x90 + channel, pitch, velocity};
    _midi->SendMessage(bytes, 3);
}

void MidiIO::sendNoteOff(const uint8_t pitch, const uint8_t channel)
{
    if (_midi == nullptr) return;
    uint8_t bytes[3] = {0x80 + channel, pitch, 0};
    _midi->SendMessage(bytes, 3);
}

void MidiIO::sendCc(uint8_t cc, uint8_t value, uint8_t channel)
{   
    if (_midi == nullptr) return;
    uint8_t bytes[3] = {0xB0 + channel, cc, value};
    _midi->SendMessage(bytes, 3);
}

void MidiIO::sendBytes(std::vector<uint8_t>& bytes)
{
    if (_midi == nullptr) return;
    _midi->SendMessage(&bytes[0], bytes.size());
}

void MidiIO::sendProgramChange(const uint8_t channel, const uint8_t program)
{
    if (_midi == nullptr) return;
    
    uint8_t bytes[3] = {0xC0 + channel, program, 0};
    _midi->SendMessage(bytes, 3);
}

} // namespace Vleerhond
