#include "polypaly.h"

using namespace daisysp;

namespace VhPatch
{
void PolyPaly::Init(float samplerate)
{
    for(int i = 0; i < NR_OF_OSCILLATORS; i++)
    {
        osc[i].Init(samplerate);
        osc[i].SetAmp(.5);
        osc[i].SetWaveform(Oscillator::WAVE_SAW); // That's saw.
    }

    svf.Init(samplerate);
    svf.SetFreq(3000);

    vca_env.Init(samplerate);
    vca_env.SetCurve(-10); // Set it fairly exponential;
    vca_env.SetTime(ADENV_SEG_ATTACK, 0.01);
    vca_env.SetTime(ADENV_SEG_DECAY, 1.0);
    vca_env.SetMax(1);
    vca_env.SetMin(0);
}

void PolyPaly::Trig(const Vleerhond::TimeStruct&    time_struct,
                    const Vleerhond::HarmonyStruct& harmony_struct)
{
    auto chord_struct                = harmony_struct.getChordStep(time_struct);
    std::vector<uint8_t> chord_notes = Vleerhond::ChordUtils::getChord(
        chord_struct, harmony_struct.scale, 60);
    for(int i = 0; i < NR_OF_OSCILLATORS; i++)
    {
        osc[i].SetFreq(mtof(chord_notes[i % NR_OF_OSCILLATORS]));
    }
    vca_env.Trigger();
}

float PolyPaly::Process()
{
    float mix = 0;
    //Process and output the three oscillators
    for(size_t chn = 0; chn < NR_OF_OSCILLATORS; chn++)
    {
        float sig = osc[chn].Process();
        mix += sig * .25f;
    }

    // Filter mix.
    svf.Process(mix);
    auto filtered = svf.Low();

    // Run Filtered through VCA.
    vca_env.Process();
    return filtered * vca_env.GetValue();
}
} // namespace VhPatch