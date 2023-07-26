#pragma once

#include "daisysp.h"

class Tom
{
  public:
    void Init(float samplerate)
    {
        tom.Init(samplerate);
        tom.SetDecay(0.5);
        tom.SetFreq(daisysp::mtof(36));
        tom.SetSustain(false);

        tom.SetSelfFmAmount(1.0);
        tom.SetAttackFmAmount(0.0);
    }

    void Trig() {
        tom.Trig();
    }

    float Process() {
        return tom.Process();
    }

    daisysp::AnalogBassDrum tom;
};