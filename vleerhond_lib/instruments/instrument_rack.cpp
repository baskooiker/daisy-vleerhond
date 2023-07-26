#include "instrument_rack.h"

namespace Vleerhond
{
InstrumentRack::InstrumentRack(TimeStruct& time) : InstrumentBase(time) {}

bool InstrumentRack::play()
{
    return getInstr()->play();
}
void InstrumentRack::randomize()
{
    // Only randomize the selected instrument in the rack.
    getInstr()->randomize();
    // for (InstrumentBase* instrument : instruments) {
    //    instrument->randomize();
    //}
}
std::vector<InstrumentBase*> InstrumentRack::getPtrs()
{
    return {getInstr()};
}
void InstrumentRack::autoRandomize()
{
    return getInstr()->autoRandomize();
}
void InstrumentRack::select(const uint8_t index)
{
    if(selection != index)
    {
        getInstr()->stopNotes();
        getInstr()->kill(false);
        this->selection = index;
    }
}
uint8_t InstrumentRack::getSelection()
{
    return this->selection;
}
void InstrumentRack::setVariableDensity(const uint8_t variable_density)
{
    getInstr()->setVariableDensity(variable_density);
}
uint8_t InstrumentRack::getVariableDensity() const
{
    return getInstr()->getVariableDensity();
}
void InstrumentRack::setVariablePitch(const uint8_t variable_pitch)
{
    getInstr()->setVariablePitch(variable_pitch);
}
uint8_t InstrumentRack::getVariablePitch() const
{
    return getInstr()->getVariablePitch();
}
void InstrumentRack::setVariablePitchOffset(const uint8_t variable_pitch_offset)
{
    getInstr()->setVariablePitchOffset(variable_pitch_offset);
}
uint8_t InstrumentRack::getVariablePitchOffset() const
{
    return getInstr()->getVariablePitchOffset();
}
void InstrumentRack::setVariableOctave(const uint8_t variable_octave)
{
    getInstr()->setVariableOctave(variable_octave);
}
uint8_t InstrumentRack::getVariableOctave() const
{
    return getInstr()->getVariableOctave();
}
void InstrumentRack::kill(const bool kill)
{
    this->_kill = kill;
    for(InstrumentBase* instr_ptr : this->instruments)
    {
        instr_ptr->kill(kill);
    }
}
void InstrumentRack::setChannel(std::shared_ptr<MidiChannel> channel)
{
    for(InstrumentBase* instr_ptr : this->instruments)
    {
        instr_ptr->setChannel(channel);
    }
}
std::shared_ptr<MidiChannel> InstrumentRack::getChannel()
{
    return getInstr()->getChannel();
}
bool InstrumentRack::getPedal()
{
    return getInstr()->getPedal();
}

std::string InstrumentRack::getName()
{
    std::string n = name;
    n.append(":");
    n.append(getInstr()->getName());
    return n;
}
InstrumentBase* InstrumentRack::getInstr()
{
    return instruments[selection % instruments.size()];
}
InstrumentBase* InstrumentRack::getInstr() const
{
    return instruments[selection % instruments.size()];
}
}; // namespace Vleerhond
