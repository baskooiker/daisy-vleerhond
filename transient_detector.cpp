#include "transient_detector.h"

namespace VhPatch
{
void TransientDetector::Init() {}

void TransientDetector::Process(float v)
{
    _prev       = _value;
    float abs_v = std::abs(v);
    if(v > _value)
    {
        _value = (_slew_up * _value) + ((1.0 - _slew_up) * abs_v);
    }
    else
    {
        _value = (_slew_down * _value) + ((1.0 - _slew_down) * abs_v);
    }
}

float TransientDetector::Value()
{
    return _value;
}

bool TransientDetector::Trig()
{
    return _value > _threshold && _prev <= _threshold;
}
} // namespace VhPatch