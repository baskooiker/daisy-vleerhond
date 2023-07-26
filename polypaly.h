#pragma once

#include "daisysp.h"

#include "harmony/harmony_struct.h"
#include "core/time_struct.h"

using namespace daisysp;

namespace VhPatch
{
class PolyPaly
{
  public:
    void  Init(float samplerate);
    void  Trig(const Vleerhond::TimeStruct&    time_struct,
               const Vleerhond::HarmonyStruct& harmony_struct);
    float Process();

  protected:
    static const uint8_t NR_OF_OSCILLATORS = 4;
    Oscillator           osc[NR_OF_OSCILLATORS];
    Svf                  svf;
    AdEnv                vca_env;
};
} // namespace VhPatch