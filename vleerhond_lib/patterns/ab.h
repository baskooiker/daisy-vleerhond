#pragma once

#include "core/defs.h"
#include <string>

namespace Vleerhond {
class AbPattern {
   protected:
    uint8_t ab_pattern[4];

   public:
    void randomize();

    void setConst(const uint8_t value = 0);

    void setLow();

    void setHigh();

    void setABCB();
    void setABCD();

    uint8_t value(const uint8_t index) const;

    bool isConstant();

    std::string toString() const;
};
}  // namespace Vleerhond