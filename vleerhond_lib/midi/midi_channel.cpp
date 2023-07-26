#include "midi/midi_channel.h"

#include "core/defs.h"
#include "core/note_struct.h"
#include "midi/midi_io.h"
#include "utils/utils.h"

#define STORAGE_SIZE 16

namespace Vleerhond
{
void MidiChannel::_sendNoteOn(const uint8_t pitch, const uint8_t velocity)
{
    if(this->channel >= 0)
    {
        MidiIO::sendNoteOn(pitch, velocity, this->channel);
    }
}

void MidiChannel::_sendNoteOff(const uint8_t pitch)
{
    if(this->channel >= 0)
    {
        MidiIO::sendNoteOff(pitch, this->channel);
    }
}

MidiChannel::MidiChannel(const uint8_t midi_channel) : channel(midi_channel) {}

void MidiChannel::processNoteEvents()
{
    uint32_t time = Utils::millis();
    for(int16_t i = this->nr_of_events - 1; i >= 0; i--)
    {
        NoteEvent event_i = this->events[i];
        if(event_i.time <= time)
        {
            this->_noteOn(event_i.notes);
            this->events[i] = this->events[this->nr_of_events - 1];
            this->nr_of_events--;
        }
    }
}

void MidiChannel::noteOff(uint8_t pitch)
{
    this->_sendNoteOff(pitch);
    NoteStruct stored = this->popFromStorage(pitch);
    (void)stored;
}

void MidiChannel::_noteOn(const std::vector<NoteStruct>& notes)
{
    if(!this->getPedal())
    {
        this->untieNotes();
    }

    for(const auto& note : notes)
    {
        // If the played note was still active, stop it before replaying
        NoteStruct stored = this->popFromStorage(note.pitch);
        if(note.pitch == stored.pitch)
        {
            this->noteOff(note.pitch);
        }
    }

    for(const auto& note : notes)
    {
        if(note.velocity > 0)
        {
            this->_sendNoteOn(note.pitch, note.velocity);
            this->addToStorage(note);
        }
    }
}

void MidiChannel::noteOn(const NoteStruct& note, const uint32_t delay)
{
    std::vector<NoteStruct> notes = {note};
    noteOn(notes, delay);
}

void MidiChannel::noteOn(const std::vector<NoteStruct>& notes,
                         const uint8_t                  delay)
{
    if(this->nr_of_events < STORAGE_SIZE && delay > 0)
    {
        NoteEvent new_event(notes, Utils::millis() + delay);
        this->events[this->nr_of_events++] = new_event;
    }
    else
    {
        this->_noteOn(notes);
    }
}

void MidiChannel::addToStorage(const NoteStruct& note)
{
    for(uint8_t i = 0; i < this->size; i++)
    {
        if(this->data[i].pitch == note.pitch)
        {
            this->data[i] = note;
            return;
        }
    }
    this->data[this->size] = note;
    this->size++;
}

NoteStruct MidiChannel::popFromStorage(uint8_t pitch)
{
    for(uint8_t i = 0; i < this->size; i++)
    {
        if(this->data[i].pitch == pitch)
        {
            NoteStruct note = this->data[i];
            this->data[i]   = this->data[this->size - 1];
            this->size--;
            return note;
        }
    }
    return NoteStruct();
}

void MidiChannel::processActiveNotes()
{
    for(uint8_t i = 0; i < (int)this->size; i++)
    {
        if(this->data[i].length > 0)
        {
            this->data[i].length -= 1;
        }
    }
    for(int16_t i = (int)this->size - 1; i >= 0; i--)
    {
        if(this->data[i].length == 0)
        {
            if(!this->pedal)
            {
                this->noteOff(this->data[i].pitch);
            }
        }
    }
}

void MidiChannel::untieNotes()
{
    for(int16_t i = (int)this->size - 1; i >= 0; i--)
    {
        if(!this->pedal)
        {
            if(this->data[i].type == NoteType::Tie)
            {
                this->noteOff(this->data[i].pitch);
            }
        }
    }
}

void MidiChannel::allNotesOff()
{
    for(int16_t i = (int)this->size - 1; i >= 0; i--)
    {
        noteOff(this->data[i].pitch);
    }
}

void MidiChannel::printStorage()
{
    printf("\nStorage size: %d\n", (int)this->size);
    for(size_t i = 0; i < this->size; i++)
    {
        printf("%2d, %3d, %d\n",
               this->data[i].pitch,
               this->data[i].length,
               (int)this->data[i].type);
    }
}

void MidiChannel::setPedal(const bool value)
{
    pedal = value;
}
bool MidiChannel::getPedal()
{
    return this->pedal;
}

// TODO: Remove when obsolete

void MidiChannel::sendCC(const uint8_t cc, const uint8_t value)
{
    uint8_t send_channel = this->channel;
    MidiIO::sendCc(cc, value, send_channel);
}
void MidiChannel::sendBytes(std::vector<uint8_t>& bytes)
{
    MidiIO::sendBytes(bytes);
}
void MidiChannel::sendProgramChange(const uint8_t program_change)
{
    uint8_t send_channel = this->channel;
    MidiIO::sendProgramChange(send_channel, program_change);
}
} // namespace Vleerhond
