#pragma once

namespace VhPatch
{
class TransientDetector
{
  public:
    void  Init();
    void  Process(float v);
    float Value();
    bool  Trig();

  protected:
    float _prev      = 0;
    float _value     = 0;
    float _threshold = 0.2;
    float _slew_up   = 0;
    float _slew_down = 0.2;
};
} // namespace VhPatch