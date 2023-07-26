#include "daisy_patch.h"
#include "daisysp.h"

#include "queue"

#include "core/time_struct.h"
#include "harmony/harmony_struct.h"
#include "instruments/minitaur/minitaur.h"
#include "instruments/monopoly/monopoly.h"
#include "midi/midi_channel.h"
#include "patterns/cv_patterns.h"
#include "patterns/modulators.h"
#include "utils/utils.h"

#include "daisy_midi_io.h"
#include "polypaly.h"
#include "tom.h"
#include "transient_detector.h"

using namespace daisy;
using namespace daisysp;

DaisyPatch hw;

Parameter control_1, control_2, control_3, control_4;

Vleerhond::TimeStruct    time_struct;
Vleerhond::HarmonyStruct harmony_struct;
Vleerhond::CvPatternAB   turing_pattern_1, turing_pattern_2;
Vleerhond::Modulators    modulators;
Vleerhond::Minitaur      minitaur(harmony_struct, modulators, time_struct);
Vleerhond::Monopoly      monopoly(harmony_struct, modulators, time_struct);

VhPatch::TransientDetector transient_detector_1, transient_detector_2,
    transient_detector_3, transient_detector_4;

bool b = false;

VhPatch::PolyPaly poly;
Tom               tom;

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    hw.ProcessAllControls();

    if(hw.encoder.RisingEdge())
    {
        // Randomize things.
        turing_pattern_1.randomize();
        turing_pattern_2.randomize();
        harmony_struct.randomize();
        minitaur.randomize();
        monopoly.randomize();
    }

    // Change file with encoder.
    auto inc = hw.encoder.Increment();
    if(inc > 0) {}
    else if(inc < 0) {}

    bool trig_1 = false, trig_2 = false, trig_3 = false, trig_4 = false;
    for(size_t i = 0; i < size; i++)
    {
        transient_detector_1.Process(in[0][i]);
        trig_1 |= transient_detector_1.Trig();
        transient_detector_2.Process(in[1][i]);
        trig_2 |= transient_detector_2.Trig();
        transient_detector_3.Process(in[2][i]);
        trig_3 |= transient_detector_3.Trig();
        transient_detector_4.Process(in[3][i]);
        trig_4 |= transient_detector_4.Trig();
    }

    if(trig_2)
    {
        poly.Trig(time_struct, harmony_struct);
    }

    auto now = System::GetNow();
    if(hw.gate_input[DaisyPatch::GATE_IN_1].Trig())
    {
        if(time_struct.state == Vleerhond::PlayState::Stopped)
        {
            time_struct.state = Vleerhond::PlayState::Playing;
            Vleerhond::MidiIO::sendStart();
            // uint8_t bytes[1]  = {0xFA}; // Start message
            // hw.midi.SendMessage(bytes, 1);
        }

        Vleerhond::MidiIO::sendTimeClock();
        // uint8_t bytes[1] = {0xF8}; // Clock message
        // hw.midi.SendMessage(bytes, 1);

        // Send CV patterns to CV outs.
        if(Vleerhond::Utils::intervalHit(Vleerhond::TimeDivision::Sixteenth,
                                         time_struct))
        {
            float voltage = turing_pattern_1.value(time_struct.step()) / 127.f;
            hw.seed.dac.WriteValue(DacHandle::Channel::ONE, voltage * 4095);
            voltage = turing_pattern_2.value(time_struct.step()) / 127.f;
            hw.seed.dac.WriteValue(DacHandle::Channel::TWO, voltage * 4095);

            static uint64_t c = 0;
            if(c % 16 == 0)
            {
                // uint8_t bytes[3] = {144, 64, 64};
                // hw.midi.SendMessage(bytes, 3);
                // Vleerhond::MidiIO::sendNoteOn(64, 64, 0);
                // monopoly.getChannel()->noteOn({Vleerhond::NoteStruct{60, 64, 6, Vleerhond::NoteType::Normal}}, 0);
            }
            if(c % 16 == 8)
            {
                // uint8_t bytes[3] = {144, 64, 0};
                // hw.midi.SendMessage(bytes, 3);
                // Vleerhond::MidiIO::sendNoteOff(64, 0);
            }
            c = (c + 1) % 16;
        }

        // Play the instruments.
        minitaur.updatePedalState();
        minitaur.getChannel()->processActiveNotes();
        minitaur.play();
        monopoly.updatePedalState();
        monopoly.getChannel()->processActiveNotes();
        monopoly.play();

        time_struct.tick += 1;
        time_struct.last_pulse_time = now;
    }

    // Play tom
    if(trig_1)
    {
        static int              tom_counter = 0;
        static std::vector<int> tom_pat     = {0, 1, 1, 0, 1};
        switch(tom_pat[tom_counter])
        {
            case 0: tom.tom.SetFreq(mtof(36)); break;
            case 1: tom.tom.SetFreq(mtof(43 - 12)); break;
            default: break;
        };
        tom_counter = (tom_counter + 1) % tom_pat.size();
        tom.Trig();
    }

    for(size_t i = 0; i < size; i++)
    {
        out[1][i] = poly.Process();
        out[0][i] = 4 * tom.Process();
    }
}

void UpdateOled()
{
    hw.display.Fill(false);

    std::string str;

    {
        char c[10];
        itoa(static_cast<int>(harmony_struct.getType()), c, 10);
        hw.display.SetCursor(0, 0);
        hw.display.WriteString(c, Font_6x8, true);

        auto chord_step = harmony_struct.getChordStep(time_struct);
        hw.display.SetCursor(16, 0);
        hw.display.WriteString(
            Vleerhond::Strings::getString(chord_step).c_str(), Font_6x8, true);

        itoa(b, c, 10);
        hw.display.SetCursor(48, 0);
        hw.display.WriteString(c, Font_6x8, true);
    }

    // Instrument rack selections;
    {
        char c[10];
        itoa(minitaur.getSelection(), c, 10);
        hw.display.SetCursor(0, 8);
        hw.display.WriteString(c, Font_6x8, true);

        itoa(monopoly.getSelection(), c, 10);
        hw.display.SetCursor(16, 8);
        hw.display.WriteString(c, Font_6x8, true);
    }

    // Display control values
    {
        char c[10];
        itoa(control_1.Value() * 127, c, 10);
        hw.display.SetCursor(0, 24);
        hw.display.WriteString(c, Font_6x8, true);

        itoa(control_2.Value() * 127, c, 10);
        hw.display.SetCursor(20, 24);
        hw.display.WriteString(c, Font_6x8, true);

        itoa(control_3.Value() * 127, c, 10);
        hw.display.SetCursor(40, 24);
        hw.display.WriteString(c, Font_6x8, true);

        itoa(control_4.Value() * 127, c, 10);
        hw.display.SetCursor(60, 24);
        hw.display.WriteString(c, Font_6x8, true);
    }

    // Display counter
    {
        uint16_t step = time_struct.step();
        for(uint64_t i = 0; i < 16; i++)
        {
            hw.display.SetCursor(i * 8 + 1, 48);
            char c = i % 4 == 0 ? 'x' : '.';
            hw.display.WriteChar(c, Font_6x8, step % 16 == i);
        }

        const uint8_t nr_of_bars     = 8;
        const uint8_t pixels_per_bar = 128 / nr_of_bars;
        uint64_t      bar            = (step / 16) % nr_of_bars;
        uint8_t       x1, y1 = 55, x2, y2 = y1 + 8;
        bool          on = true, fill = true;
        for(uint64_t i = 0; i < nr_of_bars; i++)
        {
            x1 = pixels_per_bar * i;
            x2 = x1 + pixels_per_bar;
            if(i == nr_of_bars - 1)
            {
                x2 -= 1;
            }
            fill = bar == i;
            hw.display.DrawRect(x1, y1, x2, y2, on, fill);
        }
    }
    hw.display.Update();
}

void UpdateControls()
{
    // tom.SetDecay(control_1.Process());
    // tom.SetTone(control_2.Process());
    // tom.SetSelfFmAmount(control_3.Process());
    // tom.SetAttackFmAmount(control_4.Process());

    int chord_pattern_selection = control_4.Process() * 4.99;
    switch(chord_pattern_selection)
    {
        case 0: harmony_struct.setType(Vleerhond::HarmonyType::Const); break;
        case 1: harmony_struct.setType(Vleerhond::HarmonyType::TonicLow); break;
        case 2:
            harmony_struct.setType(Vleerhond::HarmonyType::TonicHigh);
            break;
        case 3:
            harmony_struct.setType(Vleerhond::HarmonyType::DominantLow);
            break;
        case 4:
            harmony_struct.setType(Vleerhond::HarmonyType::DominantHigh);
            break;
        default: break;
    }

    int minitaur_selection = control_1.Process() * 4 * 0.999;
    minitaur.select(minitaur_selection);

    int monopoly_selection = control_2.Process() * 4 * 0.999;
    monopoly.select(monopoly_selection);
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(4); // number of samples handled per callback
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    float samplerate = hw.AudioSampleRate();

    poly.Init(samplerate);
    tom.Init(samplerate);

    control_1.Init(
        hw.controls[DaisyPatch::CTRL_1], 0.0f, 1.f, Parameter::LINEAR);
    control_2.Init(
        hw.controls[DaisyPatch::CTRL_2], 0.0f, 1.f, Parameter::LINEAR);
    control_3.Init(
        hw.controls[DaisyPatch::CTRL_3], 0.0f, 1.f, Parameter::LINEAR);
    control_4.Init(
        hw.controls[DaisyPatch::CTRL_4], 0.0f, 1.f, Parameter::LINEAR);

    transient_detector_1.Init();
    transient_detector_2.Init();
    transient_detector_3.Init();
    transient_detector_4.Init();

    turing_pattern_1.randomize();
    turing_pattern_2.randomize();

    hw.StartAdc();
    hw.StartAudio(AudioCallback);

    Vleerhond::Midi::Init(&hw.midi);

    minitaur.setChannel(std::make_shared<Vleerhond::MidiChannel>(0));
    minitaur.randomize();

    monopoly.setChannel(std::make_shared<Vleerhond::MidiChannel>(1));
    monopoly.randomize();

    while(1)
    {
        UpdateOled();
        UpdateControls();

        if(System::GetNow() - time_struct.last_pulse_time > 100)
        {
            time_struct.reset();
            if(time_struct.state == Vleerhond::PlayState::Playing)
            {
                time_struct.state = Vleerhond::PlayState::Stopped;
                Vleerhond::MidiIO::sendStop();
                minitaur.stopNotes();
                monopoly.stopNotes();
                // uint8_t bytes[1]  = {0xF8}; // Stop message
                // hw.midi.SendMer.stopNotes();
                // monopoly.stossage(bytes, 1);
            }
        }
        minitaur.getChannel()->processNoteEvents();
        monopoly.getChannel()->processNoteEvents();
    }
}

uint32_t Vleerhond::Utils::millis()
{
    return System::GetNow();
}